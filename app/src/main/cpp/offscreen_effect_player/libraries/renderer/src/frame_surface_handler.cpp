#include "frame_surface_handler.hpp"

const float bnb::render::frame_surface_handler::vertices[2][frame_surface_handler::v_size][5 * 4] =
{{ /* verical flip 0 */
{
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // top right
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f,  // top left
},
{
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  // top left
},
{
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top left
},
{
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f,  // top left
}
},
{ /* verical flip 1 */
{
        // positions        // texture coords
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,  // top left
},
{
        // positions        // texture coords
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // top right
        1.0f,  1.0f, 0.0f, 0.0f, 0.0f, // bottom right
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top left
},
{
        // positions        // texture coords
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // top right
        1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // bottom right
        -1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,  // top left
},
{
        // positions        // texture coords
        1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top right
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // bottom right
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, // bottom left
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // top left
}
}};