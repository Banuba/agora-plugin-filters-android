#pragma once

#include <bnb/spal/interfaces/all.hpp>
#include <memory>

namespace bnb::interfaces
{
    using haptic_sptr = std::shared_ptr<haptic>;
    using haptic_wptr = std::weak_ptr<haptic>;
    using haptic_uptr = std::unique_ptr<haptic>;
    using haptic_ptr = bnb::interfaces::haptic*;

}
