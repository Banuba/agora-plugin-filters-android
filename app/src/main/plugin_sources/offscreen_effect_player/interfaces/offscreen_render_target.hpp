#pragma once

#include <bnb/types/base_types.hpp>

#include "formats.hpp"

namespace bnb::interfaces
{

    class offscreen_render_target
    {
    public:
        virtual ~offscreen_render_target() = default;

        /**
         * Offscreen Render Target initialization. Includes initialization of gl context's
         * buffers and support objects.
         * Called by offscreen effect player.
         *
         * Example init()
         */
        virtual void init() = 0;

        /**
         * Offscreen Render Target deinitialization. Should be called within the same thread as init();
         * Called by offscreen effect player.
         *
         * Example init()
         */
        virtual void deinit() = 0;

        /**
         * Notify about rendering surface being resized.
         *
         * @param width New width for the rendering surface
         * @param height New height for the rendering surface
         *
         * Example surface_changed(1280, 720)
         */
        virtual void surface_changed(int32_t width, int32_t height) = 0;

        /**
         * Activate context for current thread
         *
         * Example activate_context()
         */
        virtual void activate_context() = 0;

        /**
         * Deactivate context in the corresponding thread
         * In the certain cases (GLFW on Windows) when it is intended to make OGL resource sharing
         * it is required that sharing context is not active in any thread.
         *
         * Example deactivate_context()
         */
        virtual void deactivate_context() = 0;

        /**
         * Preparing texture for effect_player
         *
         * Example prepare_rendering()
         */
        virtual void prepare_rendering() = 0;

        /**
         * Orientates the image
         *
         * @param orient
         *
         * Example prepare_rendering()
         */
        virtual void orient_image(orient_format orient) = 0;

        /**
         * Reading current buffer of active texture
         *
         * @return a data_t with bytes of the processed frame 
         *
         * Example read_current_buffer_async()
         */
        virtual bnb::data_t read_current_buffer() = 0;

        /**
         * Get texture id used for rendering of frame
         *
         * @return texture id
         *
         * Example get_current_buffer_texture()
         */
        virtual int get_current_buffer_texture() = 0;
    };
} // bnb::interfaces

using iort_sptr = std::shared_ptr<bnb::interfaces::offscreen_render_target>;
