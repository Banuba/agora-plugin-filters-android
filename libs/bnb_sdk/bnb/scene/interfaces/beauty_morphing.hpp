/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <bnb/utils/defs.hpp>

namespace bnb { namespace interfaces {

/**class which is controller for beauty morphing update. Subclass of face_morphing. */
class BNB_EXPORT beauty_morphing {
public:
    virtual ~beauty_morphing() {}

    /**
     *Sets face morphing weight.
     *@param value (float) morphing weight.
     */
    virtual void set_face_weight(float value) = 0;

    /**@return morphing weight (float). */
    virtual float get_face_weight() const = 0;

    /**
     *Sets nose morphing weight.
     *@param value (float) morphing weight.
     */
    virtual void set_nose_weight(float value) = 0;

    /**@return morphing weight (float). */
    virtual float get_nose_weight() const = 0;

    /**
     *Sets nose morphing weight.
     *@param value (float) morphing weight.
     */
    virtual void set_eyes_weight(float value) = 0;

    /**@return morphing weight (float). */
    virtual float get_eyes_weight() const = 0;
};

} }  // namespace bnb::interfaces
/// @}

