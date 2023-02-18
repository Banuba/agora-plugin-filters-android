#pragma once

#include <bnb/utils/defs.hpp>
#include <bnb/utils/interfaces/log_record_callback.hpp>

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
        utility(const std::vector<std::string>& paths, const std::string& client_token, const std::string& dump_path = std::string());
        ~utility();

#if BNB_OS_WINDOWS || BNB_OS_MACOS || BNB_OS_LINUX
        static void load_gl_functions();
#endif

        void set_log_record_callback(const std::shared_ptr<bnb::interfaces::log_record_callback>& cb, bnb::interfaces::severity_level lvl);
    };
} // namespace bnb
