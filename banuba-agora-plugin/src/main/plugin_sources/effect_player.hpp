#pragma once

#include <interfaces/effect_player.hpp>
#include <bnb/effect_player/interfaces/all.hpp>

namespace bnb::oep
{

    class effect_player : public bnb::oep::interfaces::effect_player
    {
    public:
        effect_player(int32_t width, int32_t height);

        ~effect_player();

        void surface_created(int32_t width, int32_t height) override;

        void surface_changed(int32_t width, int32_t height) override;

        void surface_destroyed() override;

        bool load_effect(const std::string& effect) override;

        bool call_js_method(const std::string& method, const std::string& param) override;

        void eval_js(const std::string& script, oep_eval_js_result_cb result_callback) override;

        void pause() override;

        void resume() override;

        void stop() override;

        void push_frame(pixel_buffer_sptr image, bnb::oep::interfaces::rotation image_orientation, bool require_mirroring) override;

        int64_t draw() override;

    private:
        std::shared_ptr<bnb::interfaces::effect_player> m_ep;
    }; /* class effect_player */

} /* namespace bnb::oep */
