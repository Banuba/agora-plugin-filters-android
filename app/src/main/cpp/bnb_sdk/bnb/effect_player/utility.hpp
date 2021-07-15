#pragma once

#include <bnb/utils/defs.hpp>
#include <bnb/utils/interfaces/log_record_callback.hpp>

#if BNB_OS_WINDOWS || BNB_OS_MACOS
    #include <glad/glad.h>
#endif

#include <vector>
#include <string>
#include <memory>

namespace bnb
{
    /*!
     * \brief The utility class is created to be used in applications which uses effect_player as
     * shared library.
     */
    class BNB_EXPORT utility
    {
    public:
        utility(const std::vector<std::string>& paths, const std::string& client_token);
        ~utility();

#if BNB_OS_WINDOWS || BNB_OS_MACOS
        static void load_glad_functions(GLADloadproc load);
#endif

        void set_log_record_callback(const std::shared_ptr<bnb::interfaces::log_record_callback>& cb, bnb::interfaces::severity_level lvl);
    };
} // namespace bnb
