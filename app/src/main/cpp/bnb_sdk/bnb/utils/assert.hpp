#pragma once

#include <cassert>
#include <bnb/utils/exceptions.hpp>

/**
 * @addtogroup utils
 * @{
 */

#define BNB_ASSERT(expr) assert(expr)
#define BNB_ASSERT_MSG(expr, msg) assert((expr) && (msg))

#define BNB_CHECK(expr)                                                         \
    if (!(expr)) {                                                              \
        BNB_THROW(std::logic_error, std::string("Condition failed: ") + #expr); \
    }
#define BNB_CHECK_MSG(expr, msg)                                      \
    if (!(expr)) {                                                    \
        BNB_THROW(std::logic_error, std::string(msg) + ": " + #expr); \
    }

/** @} */ // endgroup utils
