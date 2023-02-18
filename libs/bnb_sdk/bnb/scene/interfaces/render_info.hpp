/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <bnb/utils/defs.hpp>
#include <cstdint>
#include <string>

namespace bnb { namespace interfaces {

/** Information getters */
class BNB_EXPORT render_info {
public:
    virtual ~render_info() {}

    /** Recognition engine version */
    static int32_t get_frx_version();

    /** Platfrom name: ios, android */
    static std::string get_platform();

    /** Render engine versio. Currently the same as SDK version. E.g.: `1.2.1-g8970518da` */
    static std::string get_engine_version();
};

} }  // namespace bnb::interfaces
/// @}

