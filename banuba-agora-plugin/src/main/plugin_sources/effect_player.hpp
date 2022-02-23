#pragma once

#include <interfaces/effect_player.hpp>
#include <bnb/effect_player/interfaces/all.hpp>

namespace bnb::oep
{

    class effect_player : public bnb::oep::interfaces::effect_player
    {
    public:
        effect_player(const std::vector<std::string>& path_to_resources, const std::string& client_token);
        ~effect_player();

        void surface_created(int32_t width, int32_t height) override;
        void surface_changed(int32_t width, int32_t height) override;
        void surface_destroyed() override;

        bool load_effect(const std::string& effect) override;

        bool call_js_method(const std::string& method, const std::string& param) override;

        void pause() override;
        void resume() override;

        void push_frame(pixel_buffer_sptr image, bnb::oep::interfaces::rotation image_orientation) override;

        void draw() override;

    private:
        // temp drop-in replacement for bnb::utility due to symbol export issues
        // TODO: drop it completely and use just utility_manager instead
        class sdk_initializer
        {
        public:
            sdk_initializer(const std::vector<std::string>& paths, const std::string& client_token);
            ~sdk_initializer();
        };
        sdk_initializer m_utility;
        std::shared_ptr<bnb::interfaces::effect_player> m_ep;
        std::atomic_bool m_is_surface_created {false};
    }; /* class effect_player */

} /* namespace bnb::oep */
