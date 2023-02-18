/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <bnb/utils/defs.hpp>
#include <string>

namespace bnb { namespace interfaces {

/** Class, which resource abstraction.  */
class BNB_EXPORT resource {
public:
    virtual ~resource() {}

    virtual bool loaded() const = 0;

    virtual std::string path() const = 0;
};

} }  // namespace bnb::interfaces
/// @}

