#pragma once

#include "offscreen_effect_player.hpp"
#include "interfaces/pixel_buffer.hpp"

namespace bnb
{
    class offscreen_effect_player;
    using oep_sptr = std::shared_ptr<offscreen_effect_player>;
    using oep_wptr = std::weak_ptr<offscreen_effect_player>;

    class pixel_buffer: public interfaces::pixel_buffer
    {
    public:
        pixel_buffer(oep_sptr oep_sptr, uint32_t width, uint32_t height, camera_orientation orientation);

        void lock() override;
        void unlock() override;
        bool is_locked() override;

        void get_rgba_async(oep_image_ready_cb callback) override;
        std::optional<bnb::full_image_t> get_rgba() override;
        void get_nv12(oep_image_ready_cb callback) override;

        virtual void get_texture(oep_texture_cb callback) override;
    private:
        oep_wptr m_oep_ptr;
        uint8_t lock_count = 0;

        uint32_t m_width = 0;
        uint32_t m_height = 0;

        camera_orientation m_orientation;
    };
} // bnb