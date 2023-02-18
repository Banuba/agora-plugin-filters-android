/// \file
/// \addtogroup EffectPlayer
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from effect_player.djinni

#pragma once

#include <functional>

namespace bnb { namespace interfaces {

enum class color_range : int {
    video,
    full,
};

} }  // namespace bnb::interfaces

namespace std {

template <>
struct hash<::bnb::interfaces::color_range> {
    size_t operator()(::bnb::interfaces::color_range type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std
/// @}

