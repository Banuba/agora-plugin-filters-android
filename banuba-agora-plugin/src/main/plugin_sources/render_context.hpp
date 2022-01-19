#pragma once

#include <interfaces/render_context.hpp>

#include <EGL/egl.h>

namespace bnb::oep
{

    class render_context : public bnb::oep::interfaces::render_context
    {
    public:
        void create_context() override;
        void activate() override;
        void deactivate() override;
        void delete_context() override;

        void * get_sharing_context() override;

    private:
        EGLDisplay m_display{};
        EGLSurface m_surface{};
        EGLContext m_context{};
    }; /* class render_context */

} /* namespace bnb::oep */
