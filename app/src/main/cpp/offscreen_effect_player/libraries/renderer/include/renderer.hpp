#pragma once

#include <bnb/types/full_image.hpp>

#include "program.hpp"
#include "frame_surface_handler.hpp"

namespace bnb::render
{
    struct nv12_planes
    {
        color_plane y_plane;
        color_plane uv_plane;
    };

    class renderer
    {
    public:
        renderer(int width, int height);

        void surface_change(int32_t width, int32_t height);

        void update_data(int texture_id);
        bool draw();

    private:
        program m_program;
        frame_surface_handler m_frame_surface;

        int m_width;
        int m_height;
        int m_texture_id{ 0 };

        std::atomic<bool> m_rendering = false;
        std::atomic<bool> m_texture_updated = false;

        std::atomic<bool> m_surface_changed = false;
    };
} // bnb::render
