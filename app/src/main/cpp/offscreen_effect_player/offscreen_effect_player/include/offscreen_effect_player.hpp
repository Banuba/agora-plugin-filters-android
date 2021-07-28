#pragma once

#include <bnb/effect_player/interfaces/all.hpp>
#include <bnb/recognizer/interfaces/all.hpp>
#include "interfaces/offscreen_effect_player.hpp"
#include "interfaces/offscreen_render_target.hpp"

#include "thread_pool.h"

#include "pixel_buffer.hpp"


namespace bnb {
    class offscreen_effect_player : public interfaces::offscreen_effect_player,
                                    public std::enable_shared_from_this<offscreen_effect_player> {
    private:
        offscreen_effect_player(int32_t width, int32_t height, iort_sptr ort);

    public:
        ~offscreen_effect_player();

        void process_image_async(std::shared_ptr<full_image_t> image, oep_pb_ready_cb callback,
                                 std::optional<interfaces::orient_format> target_orient) override;

        ipb_sptr process_image(
                std::shared_ptr<full_image_t> image,
                std::optional<interfaces::orient_format> target_orient
        ) override;

        void surface_changed(int32_t width, int32_t height) override;

        void load_effect(const std::string &effect_path) override;

        void unload_effect() override;

        void pause() override;

        void resume() override;

        void enable_audio(bool enable) override;

        void call_js_method(const std::string &method, const std::string &param) override;

    private:
        friend class interfaces::offscreen_effect_player;

        friend class pixel_buffer;

        void read_current_buffer_async(std::function<void(bnb::data_t data)> callback);

        bnb::data_t read_current_buffer();

        void get_current_buffer_texture(oep_texture_cb callback);

    private:
        std::shared_ptr<interfaces::effect_player> m_ep;
        iort_sptr m_ort;

        thread_pool m_scheduler;
        std::thread::id render_thread_id;

        ipb_sptr m_current_frame;
        std::atomic<uint16_t> m_incoming_frame_queue_task_count = 0;
    };
} // bnb