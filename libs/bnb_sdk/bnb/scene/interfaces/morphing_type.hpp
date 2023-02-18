/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <functional>

namespace bnb { namespace interfaces {

enum class morphing_type : int {
    beauty,
    mesh,
    lips,
};

} }  // namespace bnb::interfaces

namespace std {

template <>
struct hash<::bnb::interfaces::morphing_type> {
    size_t operator()(::bnb::interfaces::morphing_type type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std
/// @}

