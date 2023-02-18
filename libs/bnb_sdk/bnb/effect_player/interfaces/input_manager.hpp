/// \file
/// \addtogroup EffectPlayer
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from effect_player.djinni

#pragma once

#include "bnb/types/interfaces/touch.hpp"
#include <bnb/utils/defs.hpp>
#include <cstdint>
#include <string>
#include <unordered_map>

namespace bnb { namespace interfaces {

/**
 * Class to accepts touches from user.
 *
 * Key in each map is id of an event.
 */
class BNB_EXPORT input_manager {
public:
    virtual ~input_manager() {}

    virtual void on_touches_began(const std::unordered_map<int64_t, ::bnb::interfaces::touch> & tocuhes) = 0;

    virtual void on_touches_moved(const std::unordered_map<int64_t, ::bnb::interfaces::touch> & tocuhes) = 0;

    virtual void on_touches_ended(const std::unordered_map<int64_t, ::bnb::interfaces::touch> & tocuhes) = 0;

    virtual void on_touches_cancelled(const std::unordered_map<int64_t, ::bnb::interfaces::touch> & tocuhes) = 0;

    virtual void on_rotation_gesture(float angle) = 0;

    virtual void on_scale_gesture(float scale) = 0;

    virtual void on_swipe_gesture(float dir_x, float dir_y) = 0;

    virtual void on_double_tap_gesture(const ::bnb::interfaces::touch & pos) = 0;

    virtual void on_long_tap_gesture(const ::bnb::interfaces::touch & pos) = 0;

    virtual void on_gesture_ended(const std::string & id) = 0;
};

} }  // namespace bnb::interfaces
/// @}

