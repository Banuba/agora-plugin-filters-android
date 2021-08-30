#pragma once

#include <bnb/types/base_types.hpp>

namespace bnb::interfaces {
    struct orient_format
    {
        bnb::camera_orientation orientation;
        bool is_y_flip;
    };

} // bnb::interfaces