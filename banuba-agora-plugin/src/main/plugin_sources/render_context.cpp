#include "render_context.hpp"

namespace bnb::oep
{
    /* interfaces::render_context::create */
    render_context_sptr bnb::oep::interfaces::render_context::create()
    {
        return std::make_shared<bnb::oep::render_context>();
    }

    /* render_context::create_context */
    void render_context::create_context()
    {
        const EGLint attribs[] = {EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_NONE};
        EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
        EGLDisplay display;
        EGLConfig config;
        EGLint numConfigs;
        EGLint format;
        EGLSurface surface;
        EGLContext context;

        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        eglInitialize(display, nullptr, nullptr);
        eglChooseConfig(display, attribs, &config, 1, &numConfigs);
        eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
        surface = eglCreatePbufferSurface(display, config, nullptr);
        context = eglCreateContext(display, config, nullptr, contextAttribs);

        m_display = display;
        m_surface = surface;
        m_context = context;
    }

    /* render_context::activate */
    void render_context::activate()
    {
        eglMakeCurrent(m_display, m_surface, m_surface, m_context);
    }

    /* render_context::deactivate */
    void render_context::deactivate()
    {
        eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    }

    /* render_context::delete_context */
    void render_context::delete_context()
    {
        deactivate();
        eglDestroyContext(m_display, m_context);
        eglDestroySurface(m_display, m_surface);
        eglTerminate(m_display);
    }

    /* render_context::get_sharing_context */
    void* render_context::get_sharing_context()
    {
        return m_context;
    }

} /* namespace bnb::oep */
