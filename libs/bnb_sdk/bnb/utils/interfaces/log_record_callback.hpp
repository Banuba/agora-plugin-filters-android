/// \file
/// \addtogroup Utils
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from utils.djinni

#pragma once

#include <bnb/utils/defs.hpp>
#include <string>

namespace bnb { namespace interfaces {

enum class severity_level;

class BNB_EXPORT log_record_callback {
public:
    virtual ~log_record_callback() {}

    virtual void on_log_record_callback(severity_level level, const std::string & domain, const std::string & message) = 0;
};

} }  // namespace bnb::interfaces
/// @}

