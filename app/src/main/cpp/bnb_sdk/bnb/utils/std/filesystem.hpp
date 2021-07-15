
#pragma once

#include <bnb/utils/defs.hpp>

#if BNB_OS_WINDOWS
    #include <filesystem>
#else
    #include <boost/filesystem.hpp>
#endif

// we can't pollute global namespace in header
namespace bnb
{
#if BNB_OS_WINDOWS
    namespace fs = std::filesystem;

    using error_code = std::error_code;
    using file_time_type = std::filesystem::file_time_type;

    inline fs::path replace_extension(fs::path p, const std::string& ext)
    {
        return p.replace_extension(std::string(".") + ext);
    }

    inline std::string path_to_utf8(const fs::path& p)
    {
        return std::string(reinterpret_cast<const char*>(p.generic_u8string().c_str()));
    }

    inline fs::path utf8_to_path(const std::string& s)
    {
        return fs::path(reinterpret_cast<const char8_t*>(s.c_str()));
    }
#else
    namespace fs = boost::filesystem;

    using error_code = boost::system::error_code;
    using file_time_type = std::time_t;

    inline fs::path replace_extension(const fs::path& p, const std::string& ext)
    {
        return fs::change_extension(p, ext);
    }

    inline std::string path_to_utf8(const fs::path& p)
    {
        return p.generic_string();
    }

    inline fs::path utf8_to_path(const std::string& s)
    {
        return fs::path(s);
    }
#endif
} // namespace bnb