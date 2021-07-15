#include "offscreen_effect_player.hpp"
#include "offscreen_render_target.hpp"

#include <iostream>
#include <utility>

namespace bnb {
    ioep_sptr interfaces::offscreen_effect_player::create(
            const std::vector<std::string> &path_to_resources, const std::string &client_token,
            int32_t width, int32_t height, const iort_sptr& ort) {
        if (ort == nullptr) {
            return nullptr;
        }

        // we use "new" instead of "make_shared" because the constructor in "offscreen_effect_player" is private
        return oep_sptr(new bnb::offscreen_effect_player(
                path_to_resources, client_token, width, height, ort));
    }

    offscreen_effect_player::offscreen_effect_player(
            const std::vector<std::string> &path_to_resources,
            const std::string &client_token,
            int32_t width, int32_t height,
            iort_sptr offscreen_render_target
    ) : m_ort(std::move(offscreen_render_target)),
        m_scheduler(1) {
        bnb::interfaces::utility_manager::initialize(path_to_resources, client_token);
        bnb::interfaces::effect_player_configuration conf{
                width, height,
                bnb::interfaces::nn_mode::automatically,
                bnb::interfaces::face_search_mode::good,
                false, false};
        m_ep = bnb::interfaces::effect_player::create(conf);
        auto task = [this, width, height]() {
            render_thread_id = std::this_thread::get_id();
            m_ort->init();
            m_ort->activate_context();
            m_ep->surface_created(width, height);
        };

        auto future = m_scheduler.enqueue(task);
        try {
            future.get();
        }
        catch (std::runtime_error &e) {
            std::cout << "[ERROR] Failed to initialize effect player: " << e.what() << std::endl;
            throw std::runtime_error("Failed to initialize effect player.");
        }
    }

    offscreen_effect_player::~offscreen_effect_player() {
        m_ep->surface_destroyed();
        // Deinitialize offscreen render target, should be performed on render thread.
        auto task = [this]() {
            m_ort->deinit();
        };
        m_scheduler.enqueue(task).get();
    }

    void offscreen_effect_player::process_image_async(std::shared_ptr<full_image_t> image,
                                                      oep_pb_ready_cb callback,
                                                      std::optional<interfaces::orient_format> target_orient) {
        if (m_current_frame == nullptr) {
            m_current_frame = std::make_shared<pixel_buffer>(shared_from_this(),
                                                             image->get_format().width,
                                                             image->get_format().height,
                                                             image->get_format().orientation);
        }

        if (m_current_frame->is_locked()) {
#ifdef DEBUG
            std::cout << "[Warning] The interface for processing the previous frame is lock" << std::endl;
#endif
            return;
        }

        if (!target_orient.has_value()) {
            target_orient = {image->get_format().orientation, true};
        }

        auto task = [this, image, callback, target_orient]() {
            if (m_incoming_frame_queue_task_count == 1) {
                m_current_frame->lock();

                m_ort->activate_context();
                m_ort->prepare_rendering();
                m_ep->push_frame(std::move(*image));
                while (m_ep->draw() < 0) {
                    std::this_thread::yield();
                }
                m_ort->orient_image(*target_orient);
                callback(m_current_frame);
                m_current_frame->unlock();
            } else {
                callback(std::nullopt);
            }
            --m_incoming_frame_queue_task_count;
        };

        ++m_incoming_frame_queue_task_count;
        m_scheduler.enqueue(task);
    }


    ipb_sptr offscreen_effect_player::process_image(
            std::shared_ptr<full_image_t> image,
            std::optional<interfaces::orient_format> target_orient
    ) {
        if (m_current_frame == nullptr) {
            m_current_frame = std::make_shared<pixel_buffer>(shared_from_this(),
                                                             image->get_format().width,
                                                             image->get_format().height,
                                                             image->get_format().orientation);
        }

        if (!target_orient.has_value()) {
            target_orient = {image->get_format().orientation, true};
        }

        auto task = [this, image, target_orient]() {
            m_current_frame->lock();
            m_ort->activate_context();
            m_ort->prepare_rendering();
            m_ep->push_frame(std::move(*image));
            while (m_ep->draw() < 0) {
                std::this_thread::yield();
            }
            m_ort->orient_image(*target_orient);
            return m_current_frame;
        };
        return m_scheduler.enqueue(task).get();
    }

    void offscreen_effect_player::surface_changed(int32_t width, int32_t height) {
        auto task = [this, width, height]() {
            m_ort->activate_context();

            m_ep->surface_changed(width, height);
            m_ep->effect_manager()->set_effect_size(width, height);

            m_current_frame.reset();
            m_ort->surface_changed(width, height);
        };

        m_scheduler.enqueue(task);
    }

    void offscreen_effect_player::load_effect(const std::string &effect_path) {
        auto task = [this, effect = effect_path]() {
            m_ort->activate_context();

            if (auto e_manager = m_ep->effect_manager()) {
                e_manager->load(effect);
            } else {
                std::cout << "[Error] effect manager not initialized" << std::endl;
            }
        };

        m_scheduler.enqueue(task);
    }

    void offscreen_effect_player::unload_effect() {
        load_effect("");
    }

    void offscreen_effect_player::pause() {
        m_ep->playback_pause();
    }

    void offscreen_effect_player::resume() {
        m_ep->playback_play();
    }

    void offscreen_effect_player::enable_audio(bool enable) {
        m_ep->set_effect_volume(enable ? 1 : 0);
    }

    void
    offscreen_effect_player::call_js_method(const std::string &method, const std::string &param) {
        auto task = [this, method = method, param = param]() {
            m_ort->activate_context();

            if (auto e_manager = m_ep->effect_manager()) {
                if (auto effect = e_manager->current()) {
                    effect->call_js_method(method, param);
                } else {
                    std::cout << "[Error] effect not loaded" << std::endl;
                }
            } else {
                std::cout << "[Error] effect manager not initialized" << std::endl;
            }
        };

        m_scheduler.enqueue(task);
    }

    void offscreen_effect_player::read_current_buffer_async(
            std::function<void(bnb::data_t data)> callback) {
        if (std::this_thread::get_id() == render_thread_id) {
            callback(m_ort->read_current_buffer());
            return;
        }

        oep_wptr this_ = shared_from_this();
        auto task = [this_, callback]() {
            if (auto this_sp = this_.lock()) {
                callback(this_sp->m_ort->read_current_buffer());
            }
        };
        m_scheduler.enqueue(task);
    }

    bnb::data_t offscreen_effect_player::read_current_buffer() {
        if (std::this_thread::get_id() == render_thread_id) {
            return m_ort->read_current_buffer();
        }
        auto task = [this]() {
            return this->m_ort->read_current_buffer();
        };
        return m_scheduler.enqueue(task).get();
    }

    void offscreen_effect_player::get_current_buffer_texture(oep_texture_cb callback) {
        if (std::this_thread::get_id() == render_thread_id) {
            callback(m_ort->get_current_buffer_texture());
            return;
        }

        oep_wptr this_ = shared_from_this();
        auto task = [this_, callback]() {
            if (auto this_sp = this_.lock()) {
                callback(this_sp->m_ort->get_current_buffer_texture());
            }
        };
        m_scheduler.enqueue(task);
    }

} // bnb
