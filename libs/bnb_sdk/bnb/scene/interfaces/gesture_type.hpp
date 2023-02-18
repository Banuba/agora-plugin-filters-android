/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <functional>

namespace bnb { namespace interfaces {

enum class gesture_type : int {
    no_gesture,
    like,
    ok,
    palm,
    rock,
    victory,
};

} }  // namespace bnb::interfaces

namespace std {

template <>
struct hash<::bnb::interfaces::gesture_type> {
    size_t operator()(::bnb::interfaces::gesture_type type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std
/// @}

