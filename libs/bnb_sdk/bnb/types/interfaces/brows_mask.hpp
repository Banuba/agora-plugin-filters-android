/// \file
/// \addtogroup Types
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from types.djinni

#pragma once

#include "bnb/types/interfaces/transformed_mask_byte.hpp"
#include <utility>

namespace bnb { namespace interfaces {

struct brows_mask final {
    transformed_mask_byte left;
    transformed_mask_byte right;

    brows_mask(transformed_mask_byte left_,
               transformed_mask_byte right_)
    : left(std::move(left_))
    , right(std::move(right_))
    {}
};

} }  // namespace bnb::interfaces
/// @}

