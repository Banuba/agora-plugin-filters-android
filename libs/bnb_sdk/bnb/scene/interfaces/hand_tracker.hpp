/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <bnb/utils/defs.hpp>
#include <memory>

namespace bnb { namespace interfaces {

class component;
class hand;
enum class hand_tracker_type;

class BNB_EXPORT hand_tracker {
public:
    virtual ~hand_tracker() {}

    /**
     *Fabric method for hand creating.
     *@return created (hand_tracker).
     */
    static std::shared_ptr<hand_tracker> create();

    /**
     *Sets hand to hand tracker.
     *@param hand (hand): hand asset
     */
    virtual void set_hand(const std::shared_ptr<hand> & hand) = 0;

    /**@return hand (hand): hand asset */
    virtual std::shared_ptr<hand> get_hand() = 0;

    /**@return tracker type. Can be hand_skelet or ring */
    virtual hand_tracker_type get_type() = 0;

    /**@return tracker type. Can be hand_skelet or ring */
    virtual void set_type(hand_tracker_type type) = 0;

    /**@return upcast to base type */
    virtual std::shared_ptr<component> as_component() = 0;
};

} }  // namespace bnb::interfaces
/// @}

