/// \file
/// \addtogroup Scene
/// @{
///
// AUTOGENERATED FILE - DO NOT MODIFY!
// This file was generated by Djinni from scene.djinni

#pragma once

#include <utility>

namespace bnb { namespace interfaces {

struct mat4x4 final {
    float m11;
    float m12;
    float m13;
    float m14;
    float m21;
    float m22;
    float m23;
    float m24;
    float m31;
    float m32;
    float m33;
    float m34;
    float m41;
    float m42;
    float m43;
    float m44;

    mat4x4(float m11_,
           float m12_,
           float m13_,
           float m14_,
           float m21_,
           float m22_,
           float m23_,
           float m24_,
           float m31_,
           float m32_,
           float m33_,
           float m34_,
           float m41_,
           float m42_,
           float m43_,
           float m44_)
    : m11(std::move(m11_))
    , m12(std::move(m12_))
    , m13(std::move(m13_))
    , m14(std::move(m14_))
    , m21(std::move(m21_))
    , m22(std::move(m22_))
    , m23(std::move(m23_))
    , m24(std::move(m24_))
    , m31(std::move(m31_))
    , m32(std::move(m32_))
    , m33(std::move(m33_))
    , m34(std::move(m34_))
    , m41(std::move(m41_))
    , m42(std::move(m42_))
    , m43(std::move(m43_))
    , m44(std::move(m44_))
    {}
};

} }  // namespace bnb::interfaces
/// @}

