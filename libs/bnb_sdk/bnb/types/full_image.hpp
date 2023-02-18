#pragma once

#if BNB_SDK_PROJECT
    #include <bnb/types/config.hpp>
#endif
#include <bnb/types/interfaces/pixel_format.hpp>
#include <bnb/types/base_types.hpp>
#include <bnb/types/transformation.hpp>
#include <bnb/utils/assert.hpp>
#include <bnb/utils/event.hpp>
#include <bnb/utils/defs.hpp>

#include <type_traits>
#include <variant>

namespace bnb
{
    /**
     * @addtogroup types
     * @{
     */

    /**
     * A structure stores format information about image.
     *
     */
    struct BNB_EXPORT image_format
    {
        /** It is a width of image in pixels. */
        uint32_t width = 0;
        /** It is a height of image in pixels. */
        uint32_t height = 0;
        /** It is a field of view in degrees. */
        std::optional<float> fov = std::nullopt;

        /** It is a camera orientation in degrees as enum value (@see camera_orientation). */
        camera_orientation orientation = camera_orientation::deg_270;

        /** This value is responsible for mirroring image during rendering. */
        bool require_mirroring = false;
        /** It is a face orientation in degrees. */
        int face_orientation = 0;

        /**
         * @return Returns the image size in pixels, i.e. width * height.
         */
        uint32_t size() const noexcept;

        /**
         * Construct a new image format object with default values.
         *
         */
        image_format() = default;

        /**
         * Construct a new image format object with user's params.
         *
         * @param width is a width of image in pixels.
         * @param height is a height of image in pixels.
         * @param orientation is a camera orientation in degrees as enum value (@see camera_orientation).
         * @param require_mirroring is a variable responsible for mirroring image during rendering, i.e. if it is true the image will mirror during rendering and won't in another way.
         * @param face_orientation is a face orientation in degrees.
         * @param fov is a field of view in degrees.
         */
        image_format(uint32_t width, uint32_t height, camera_orientation orientation, bool require_mirroring, int face_orientation, std::optional<float> fov = std::nullopt);
    };


    /**
     * A base class for *_image_t classes.
     *
     */
    class BNB_EXPORT base_image_t
    {
    public:
        /**
         * @brief Construct a new base_image_t object with default values.
         *
         */
        base_image_t() = default;

        /**
         *
         * @return Returns a const reference for image format (@see image_format)
         */
        const image_format& get_format() const;

        /**
         * Set the field of view.
         *
         * @param fov is a field of view in degrees.
         */
        void set_fov(float fov)
        {
            m_format.fov = fov;
        }

    protected:
        explicit base_image_t(const image_format& format);

        image_format m_format;
    };

    /**
     * A class for representing bpc8 images as C++ object.
     *
     */
    class BNB_EXPORT bpc8_image_t final : public base_image_t
    {
    public:
        using pixel_format_t = bnb::interfaces::pixel_format; /** enum value represents pixel format (@see pixel_format) */

        /**
         * Construct a new bpc8_image_t object with default values.
         *
         */
        bpc8_image_t() = default;

        /**
         * Construct a new bpc8_image_t object using move semantics.
         *
         */
        bpc8_image_t(bpc8_image_t&&) = default;
        bpc8_image_t& operator=(bpc8_image_t&& other) = default;

        /**
         * Construct a new bpc8_image_t object using copy operation.
         *
         */
        bpc8_image_t(const bpc8_image_t&) = default;
        bpc8_image_t& operator=(const bpc8_image_t&) = default;

        /**
         * @brief Construct a new bpc8_image_t object with user's values.
         *
         * @param data is a color plane data (@see color_plane).
         * @param type is a pixel format as enum value (@see pixe_format_t).
         * @param format is a image format (@see image_format).
         */
        bpc8_image_t(color_plane data, pixel_format_t type, const image_format& format);

        /**
         * @return Returns the pixel format as enum value (@see pixe_format_t).
         *
         */
        pixel_format_t get_pixel_format() const noexcept;

        /**
         * @return Returns a raw pointer on data of image.
         */
        uint8_t* get_data() const noexcept;

        /**
         * @return Returns count of bytes per pixel for pixel format (@p fmt).
         */
        static uint8_t bytes_per_pixel(pixel_format_t fmt);
        /**
         * @return Returns offsets for every component of rgb in bytes.
         */
        static std::tuple<int, int, int> rgb_offsets(pixel_format_t fmt);

        /**
         * @return Returns RGB pixel at position
         */
        std::tuple<uint8_t, uint8_t, uint8_t> rgb_pixel_at(uint32_t x, uint32_t y) const noexcept;

        /**
         * Normalize image data if it is required to be mirrored or camera orientation isn't deg_0.
         */
        void normalize_orientation(bnb::transformation const& basis_transform, bnb::pixel_rect const& full_roi);

    private:
        pixel_format_t m_pixel_format;
        color_plane m_data;
    };

    /**
     * Enum class represents color range.
     *
     */
    enum class color_range
    {
        video, /** for y values range is 16-235, for u and v values range is 16-240 */
        full,  /** for y, u and v values range is 0-255 */
    };

    /**
     * Enum class represents suppotred color standarts.
     *
     */
    enum class color_std
    {
        bt601,
        bt709,
    };

    /**
     * Enum class represents supported yuv formats.
     *
     */
    enum class yuv_format
    {
        yuv_nv12,
        yuv_i420,
    };

    /**
     * Struture stores all information about yuv format.
     *
     */
    struct yuv_format_t
    {
        color_range range;  // i.e. "full" or "video"
        color_std standard; // i.e. BT.601 or BT.709
        yuv_format format;  // i.e. NV12 or I420
    };

    /**
     * Clas represents yuv image as C++ object.
     *
     */
    class BNB_EXPORT yuv_image_t final : public base_image_t
    {
    public:
        /**
         * @return Returns the color plane by its object and yuv image format.
         */
        template<size_t index>
        const color_plane get_plane() const
        {
            switch (index) {
                case 0: {
                    return m_planes[0];
                }
                case 1: {
                    return m_planes[1];
                }
                case 2: {
                    switch (m_yuv_format.format) {
                        case yuv_format::yuv_nv12: {
                            BNB_THROW(std::invalid_argument, "yuv nv12 format has only 2 planes");
                        }
                        case yuv_format::yuv_i420: {
                            return m_planes[2];
                        }
                        default: {
                            BNB_THROW(std::invalid_argument, "Incorrect yuv format");
                        }
                    }
                }
                default: {
                    BNB_THROW(std::invalid_argument, "Incorrect index of plane");
                }
            }
        };

        /**
         * Construct a new yuv_image_t object with user's values (for yuv_nv12 format).
         *
         * @param y_plane is the zeroyth plane of image.
         * @param uv_plane is the uv plane of image.
         * @param format is a image format (@see image_format).
         * @param yuv_format is a yuv image format (@see yuv_image_t).
         */
        yuv_image_t(color_plane y_plane, color_plane uv_plane, const image_format& format, const yuv_format_t& yuv_format)
            : base_image_t(format)
            , m_yuv_format(yuv_format)
        {
            m_planes[0] = std::move(y_plane);
            m_planes[1] = std::move(uv_plane);
        }

        /**
         * Construct a new yuv image t object with user's values (for yuv_i420 format).
         *
         * @param y_plane is the y plane of image.
         * @param u_plane is the u plane of image.
         * @param v_plane is the v plane of image.
         * @param format is a image format (@see image_format).
         * @param yuv_format is a yuv image format (@see yuv_image_t).
         */
        yuv_image_t(color_plane y_plane, color_plane u_plane, color_plane v_plane, const image_format& format, const yuv_format_t& yuv_format)
            : base_image_t(format)
            , m_yuv_format(yuv_format)
        {
            m_planes[0] = std::move(y_plane);
            m_planes[1] = std::move(u_plane);
            m_planes[2] = std::move(v_plane);
        }

        /**
         * Construct a new yuv image t object wioth user's values but with default yuv image format.
         *
         * @param y_plane is the y plane of image.
         * @param uv_plane is the uv plane of image.
         * @param format is a image format (@see image_format).
         */
        yuv_image_t(color_plane y_plane, color_plane uv_plane, const image_format& format)
            : base_image_t(format)
        {
            m_yuv_format.range = color_range::full;
            m_yuv_format.format = yuv_format::yuv_nv12;
            m_yuv_format.standard = color_std::bt601;

            m_planes[0] = std::move(y_plane);
            m_planes[1] = std::move(uv_plane);
        }

        /**
         * @return Returns the size of y plane of image
         */
        size_t y_size() const noexcept;
        /**
         * @return Returns the size of uv plane of image.
         */
        size_t uv_size() const noexcept;

        using conversion_matrix = std::array<float, 16>;

        // NOTE: Matrix should be transposed to multiply vector-row by matrix.
        virtual conversion_matrix get_yuv_to_rgb_matrix() const;
        virtual conversion_matrix get_rgb_to_yuv_matrix() const;

        /**
         * @return Returns the pixel's value of y plane with coords @p yi and @p xi.
         */
        uint8_t y_pixel_at(uint32_t yi, uint32_t xi) const noexcept;
        /**
         * @return Returns the pixel's value of uv plane with coords @p yi and @p xi.
         */
        uint8_t uv_pixel_at(uint32_t yi, uint32_t xi, uint32_t offset) const noexcept;

        /**
         * @return Returns the pixel's value of u plane with coords @p yi and @p xi.
         */
        uint8_t u_pixel_at(uint32_t yi, uint32_t xi) const noexcept;
        /**
         * @return Returns the pixel's value of v plane with coords @p yi and @p xi.
         */
        uint8_t v_pixel_at(uint32_t yi, uint32_t xi) const noexcept;

        /**
         * Construct a new yuv_image_t object with default params.
         *
         */
        yuv_image_t() = default;

        /**
         * Construct a new yuv_image_t object using move semantics.
         *
         */
        yuv_image_t(yuv_image_t&&) = default;
        yuv_image_t& operator=(yuv_image_t&& other) = default;
        /**
         * Construct a new yuv image t object with copy operation.
         *
         */
        yuv_image_t(const yuv_image_t&) = default;
        yuv_image_t& operator=(const yuv_image_t&) = default;

        /**
         * Normalize image data if it is required to be mirrored or camera orientation isn't deg_0.
         */
        void normalize_orientation(bnb::transformation const& y_basis, bnb::transformation const& uv_basis, bnb::pixel_rect const& full_roi);

        /**
         * @return Returns the const reference on yuv_format of image (@see yuv_format_t).
         */
        const yuv_format_t& get_yuv_format() const noexcept;

        /**
         * @return Returns conversion matrix (@see conversion_matrix) for @p standard (@see color_std) and @p range (@see color_range) from yuv to rgb.
         */
        static const bnb::yuv_image_t::conversion_matrix& get_yuv_to_rgb_matrix(color_std standard, color_range range);
        /**
         * @return Returns conversion matrix (@see conversion_matrix) for @p standard (@see color_std) and @p range (@see color_range) from rgb to yuv.
         */
        static const bnb::yuv_image_t::conversion_matrix& get_rgb_to_yuv_matrix(color_std standard, color_range range);

    private:
        yuv_format_t m_yuv_format;

        //[3] planes is optional for i420 formats
        color_plane m_planes[3];
    };

    /// basis is the base basis:
    /// for y/rgb basis use .basis or get_subchannel_basis_transform(1);
    /// for uv basis use get_subchannel_basis_transform(2);
    class BNB_EXPORT full_image_t : public transformable_event<full_image_t>
    {
    public:
        /**
         * A method for loading video from @p path with/whithout @p alpha.
         *
         * @param path is a full path to downloadable image.
         * @param alpha is value responcible that image includes alpha channel.
         * @return Returns full_image_t object with data of image from @p path or nullptr.
         */
        static full_image_t load(const std::string& path, bool alpha = false);

        /**
         * Construct a new full_image_t object with default params.
         *
         */
        full_image_t();

        /**
         * Construct a new full_image_t object from @p image.
         *
         * @param image is a C++ bpc8 image ( @see bpc8_image ) representation.
         */
        explicit full_image_t(bpc8_image_t image);
        /**
         * Construct a new full_image_t object from @p image.
         *
         * @param image is a C++ yuv image ( @see yuv_image ) representation.
         */
        explicit full_image_t(yuv_image_t image);

        ~full_image_t() override;

        /**
         * Construct a new full_image_t object using copy operation.
         *
         */
        full_image_t(const full_image_t&);
        full_image_t& operator=(const full_image_t&);

        /**
         * Construct a new full_image_t object using move semantics.
         *
         */
        full_image_t(full_image_t&&) noexcept;
        full_image_t& operator=(full_image_t&& other) noexcept;

        /**
         * @return Returns image format ( @see image_format ).
         */
        image_format get_format() const;
        /**
         * @return true if image orientation is landscape and false in another way.
         */
        bool is_landscape() const;

        bnb::transformation get_subchannel_basis_transform(float inv_scale = 1.f) const
        {
            return basis_transform >> bnb::transformation(1.f / inv_scale, 1.f / inv_scale);
        }

        /**
         * @return Returns bassis of image as row-major matrix 3x3 ( @see transformation ).
         */
        bnb::transformation image_basis() const
        {
            using rot_t = transformation::rotate_t;
            return bnb::transformation(full_roi, full_roi, rot_t::deg_0, get_format().require_mirroring) >> basis_transform;
        }

        /**
         * @return Returns the image object if T is yuv or bpc8 image and at the same time yuv or bpc8 image saved.
         */
        template<typename T>
        const T& get_data() const noexcept;

        /**
         * @return Returns true if T is yuv or bpc8 image and at the same time yuv or bpc8 image saved.
         */
        template<typename T>
        bool has_data() const noexcept;

        /**
         * @brief Set the field of view ( @p fov ) object.
         *
         * @param fov is a field of view in degrees.
         */
        void set_fov(float fov);

        /**
         * Normalize image data if it is required to be mirrored or camera orientation isn't deg_0.
         */
        void normalize_orientation();

#if BNB_OS_ANDROID
        unsigned ext_camera_tex_id = 0;     /** external texture for optimisation on android (check this PR: https://bitbucket.org/BanubaLimited/banuba_sdk/pull-requests/4391 )*/
        unsigned ext_camera_tex_width = 0;  /** Width of external texture in pixels. */
        unsigned ext_camera_tex_height = 0; /** Height of external texture in pixels. */
#endif

    private:
        using image_t = std::variant<yuv_image_t, bpc8_image_t>;
        image_t m_image;

        template<typename T>
        const T& _get_data() const noexcept;

        template<typename T>
        bool _has_data() const noexcept;

        void update_basis_transform();
    };

    template<typename T>
    inline const T& bnb::full_image_t::get_data() const noexcept
    {
        static_assert(std::is_base_of<base_image_t, T>::value, "Type is not image_t");
        BNB_ASSERT(std::holds_alternative<T>(m_image));
        return *std::get_if<T>(&m_image);
    }

    template<typename T>
    inline bool full_image_t::has_data() const noexcept
    {
        static_assert(std::is_base_of<base_image_t, T>::value, "Type is not image_t");
        return std::holds_alternative<T>(m_image);
    }

    /** @} */ // endgroup types

} // namespace bnb
