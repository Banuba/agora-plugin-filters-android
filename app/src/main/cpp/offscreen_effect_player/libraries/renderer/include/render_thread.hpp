#pragma once

#include <GLES3/gl3.h>
#include <EGL/egl.h>

#include <thread>

#include "renderer.hpp"

namespace bnb::render {
    class render_thread {
    public:
        render_thread(EGLDisplay *display, EGLSurface *window, EGLContext *context, int32_t width, int32_t height);

        ~render_thread();

        void surface_changed(int32_t width, int32_t height);

        void update_data(int texture_id);

    private:
        void thread_func(int32_t width, int32_t height);

        std::unique_ptr<renderer> m_renderer{nullptr};
        EGLDisplay *m_display;
        EGLSurface *m_surface;
        EGLContext *m_context;
        std::thread m_thread;
        std::atomic<bool> m_cancellation_flag;
    };
} // bnb::render

using render_t_sptr = std::shared_ptr<bnb::render::render_thread>;
using render_t_wptr = std::weak_ptr<bnb::render::render_thread>;