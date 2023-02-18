#pragma once

#if BNB_SDK_PROJECT
    #include <bnb/utils/defs.hpp>
    #include <bnb/types/config.hpp>
    #include <bnb/utils/assert.hpp>
    #include <bnb/utils/logger.hpp>
#endif

#include <vector>
#include <string>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>
#include <functional>

namespace bnb
{
    /**
     * @defgroup types Types
     * @{
     */

    /// camera image layout is top-left, 0 orientation is portrait, rotation is counterclockwise
    enum class camera_orientation : unsigned int
    {
        deg_0 = 0,
        deg_90 = 1,
        deg_180 = 2,
        deg_270 = 3,
    };
    using high_res_timer = std::chrono::high_resolution_clock;
    using time_stamp_t = std::chrono::time_point<std::chrono::high_resolution_clock>;

    union point2d
    {
        struct
        {
            float x, y;
        };
        float data[2];

        float sum()
        {
            return x + y;
        }

        point2d sqr() const noexcept
        {
            return *this * *this;
        }

        point2d operator/(const point2d& p) const noexcept
        {
            return point2d{x / p.x, y / p.y};
        }

        point2d operator/(float f) const noexcept
        {
            return point2d{x / f, y / f};
        }

        point2d operator*(const point2d& p) const noexcept
        {
            return point2d{x * p.x, y * p.y};
        }

        point2d operator-(const point2d& p) const noexcept
        {
            return point2d{x - p.x, y - p.y};
        }

        point2d operator+(const point2d& p) const noexcept
        {
            return point2d{x + p.x, y + p.y};
        }
    };

    union point3d
    {
        struct
        {
            float x, y, z;
        };
        float data[3];
    };

    struct size
    {
        uint32_t width;
        uint32_t height;
    };

    struct data_t
    {
        using type = uint8_t[];
        using pointer = uint8_t*;
        using uptr = std::unique_ptr<type, std::function<void(pointer)>>;
        uptr data;
        size_t size;

        static data_t create_non_owning(std::shared_ptr<uint8_t> datum, size_t size)
        {
            return data_t{
                uptr(datum.get(), [](pointer) { /* don't release */ }),
                size};
        }

        static data_t create_non_owning(uint8_t* datum, size_t size)
        {
            return data_t{
                uptr(datum, [](pointer) {}),
                size};
        }

        void reserve(size_t reserve_size)
        {
            if (size < reserve_size) {
                data = std::make_unique<type>(reserve_size);
                size = reserve_size;
            }
        }
        data_t() = default;

        data_t(uptr d, size_t s)
            : data(std::move(d))
            , size(s)
        {
        }

        data_t(data_t&&) = default;
        data_t& operator=(data_t&&) = default;

#if BNB_PYBIND_BUILD || BNB_OS_EMSCRIPTEN
        // WARNING: it is NOT deep copy constructor, it provides weak (non-owning) copy
        data_t(const data_t& other)
            : data(other.data.get(), [](pointer) { /* DO NOTHING */ })
            , size(other.size)
        {
            BNB_ASSERT_MSG(false, "bnb::data_t copy constructor!");
            bnb::logger::print(bnb::severity_level::warning, "bnb::data_t", "copy constructor called, possible memory issue!");
        }

        // WARNING: it is NOT deep copy operator, it provides weak (non-owning) copy
        data_t& operator=(const data_t& other)
        {
            BNB_ASSERT_MSG(false, "bnb::data_t copy operator!");
            bnb::logger::print(bnb::severity_level::warning, "bnb::data_t", "copy operator called, possible memory issue!");
            data = uptr(other.data.get(), [](pointer) { /* DO NOTHING */ });
            size = other.size;
            return *this;
        }
#endif
    };

    using color_plane_data_t = std::uint8_t;
    using color_plane = std::shared_ptr<color_plane_data_t>;

    inline color_plane color_plane_vector(std::vector<color_plane_data_t> vector)
    {
        // please, don't do stuff like ->> again..  //return color_plane(vector.data(), [v =
        // std::move(vector)](color_plane_data_t*) {}) are you REALLY sure no one is going to copy
        // this deleter internally? (like shared_ptr does)
        auto* ptr = new std::vector<color_plane_data_t>(std::move(vector));
        return color_plane(ptr->data(), [ptr](color_plane_data_t*) { delete ptr; });
    }

    inline color_plane color_plane_string(std::string str)
    {
        auto* ptr = new std::string(std::move(str));
        return color_plane(reinterpret_cast<color_plane_data_t*>(ptr->data()), [ptr](color_plane_data_t*) { delete ptr; });
    }

    inline color_plane color_plane_char_arr(char* ptr, size_t size)
    {
        std::vector<color_plane_data_t> plane(size);
        memcpy(plane.data(), reinterpret_cast<color_plane_data_t*>(ptr), size);
        return color_plane_vector(plane);
    }

    inline color_plane color_plane_weak(const color_plane_data_t* ptr)
    {
        return color_plane(const_cast<color_plane_data_t*>(ptr), [](color_plane_data_t*) { /* DO NOTHING */ });
    }


    inline color_plane color_plane_data(data_t&& data)
    {
        auto* ptr = new data_t(std::move(data));
        return color_plane(ptr->data.get(), [ptr](color_plane_data_t*) { delete ptr; });
    }

    /** @} */ // endgroup types
} // namespace bnb
