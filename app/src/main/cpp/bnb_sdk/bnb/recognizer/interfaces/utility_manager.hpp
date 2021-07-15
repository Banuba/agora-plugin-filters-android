/// \file
/// \addtogroup Recognizer
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from recognizer.djinni

#pragma once

#include "bnb/utils/interfaces/hardware_class.hpp"
#include "bnb/utils/interfaces/log_record_callback.hpp"
#include "bnb/utils/interfaces/severity_level.hpp"
#include <bnb/utils/defs.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace bnb { namespace interfaces {

class BNB_EXPORT utility_manager {
public:
    virtual ~utility_manager() {}

    static void initialize(const std::vector<std::string> & resource_paths, const std::string & client_token);

    static void release();

    /** Diagnostic crash. Use this to check you crash analytics systems. */
    static void diagnostic_crash();

    /**
     * Get information about CPU capabilities. Mainly for usage on Android
     * @return JSON-string describing CPU capabilties
     */
    static std::string cpu_env();

    /**
     * Check device SoC class.
     * Only Android supported now, for other platforms always return hardware_class::high
     * @return hardware class of device
     */
    static ::bnb::interfaces::hardware_class get_hardware_class();

    static void set_log_level(::bnb::interfaces::severity_level level);

    /**
     * Set callback to recieve log events from Banuba SDK.
     * @param cb Callback
     * @param lvl recieve logs with specified and higher levels
     */
    static void set_log_record_callback(const std::shared_ptr<::bnb::interfaces::log_record_callback> & cb, ::bnb::interfaces::severity_level lvl);

    /** Returns SDK version as string */
    static std::string get_banuba_sdk_version_string();

    /**
     * Returns SDK version as number
     * major = version / 10000000
     * minor = (version - major * 10000000) / 100000
     * patch = (version - major * 10000000 - minor * 100000) / 1000
     * revision = version % 1000
     */
    static int32_t get_banuba_sdk_version();

    /**
     * load GL functions, for dynamic libraries only
     * should be called with active GL context
     */
    static void load_gl_functions();
};

} }  // namespace bnb::interfaces
/// @}

