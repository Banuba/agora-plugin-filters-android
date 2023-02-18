
#pragma once

#include <bnb/utils/defs.hpp>

#include <filesystem>

// we can't pollute global namespace in header
namespace bnb
{
    namespace fs = std::filesystem;
    using error_code = std::error_code;
    using file_time_type = std::filesystem::file_time_type;

    inline fs::path replace_extension(fs::path p, const std::string& ext)
    {
        return p.replace_extension(std::string(".") + ext);
    }

#if BNB_OS_WINDOWS
    inline std::string path_to_utf8(const fs::path& p)
    {
        return std::string(reinterpret_cast<const char*>(p.generic_u8string().c_str()));
    }

    inline fs::path utf8_to_path(const std::string& s)
    {
        return fs::path(reinterpret_cast<const char8_t*>(s.c_str()));
    }
#else
    inline std::string path_to_utf8(const fs::path& p)
    {
        return p.generic_string();
    }

    inline fs::path utf8_to_path(const std::string& s)
    {
        return fs::path(s);
    }
#endif /* #if BNB_OS_WINDOWS */

    inline std::string remove_trailing_separator(std::string strpath)
    {
        if (!strpath.empty() && (fs::path::preferred_separator == strpath[strpath.size() - 1])) {
            strpath.erase(strpath.size() - 1);
        }
        return strpath;
    }

    template<typename... T>
    bool match_extesions(const fs::path& path, T&&... extensions)
    {
        auto ext = bnb::path_to_utf8(path.extension());
        std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c) { return std::tolower(c); });
        bool match = false;
        ([&]() {
            if (match) {
                return;
            }
            std::string ext_to_lower(extensions);
            std::transform(ext_to_lower.begin(), ext_to_lower.end(), ext_to_lower.begin(), [](unsigned char c) { return std::tolower(c); });
            match = ext == ext_to_lower;
        }(),
         ...);

        return match;
    }
} // namespace bnb
