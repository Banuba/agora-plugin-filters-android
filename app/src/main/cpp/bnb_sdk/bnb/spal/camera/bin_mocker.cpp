#include <bnb/spal/camera/bin_mocker.hpp>
#include <bnb/types/constants.hpp>
#include <bnb/types/base_types.hpp>
#include <bnb/utils/assert.hpp>
#include <bnb/utils/std/filesystem.hpp>

using namespace bnb;

namespace
{
    std::queue<std::string> get_files(std::string input_dir)
    {
        fs::path dir_path(input_dir);

        BNB_ASSERT(fs::is_directory(dir_path));

        std::vector<std::string> bin_files;
        for (auto&& x : fs::directory_iterator(dir_path)) {
            if (x.path().extension() == ".bin") {
                bin_files.push_back(x.path().string());
            }
        }

        std::sort(bin_files.begin(), bin_files.end());

        std::queue<std::string> output_q;
        for (auto& path : bin_files) {
            output_q.emplace(path);
        }
        return output_q;
    }
} // namespace

camera_bin_mocker::camera_bin_mocker(const camera_base::push_frame_cb_t& cb, const std::string& path, size_t width, size_t height, size_t an_offset)
    : camera_base_mocker(cb)
    , m_bin_files(get_files(path))
    , m_width(width)
    , m_height(height)
    , m_offest(an_offset)
{
    start();
}

void camera_bin_mocker::update_image_content()
{
    std::vector<uint8_t> m_y_plane(m_width * m_height), m_uv_plane(m_width * m_height * 2 / 2 / 2);

    if (m_bin_files.empty()) {
        abort();
        return;
    }

    fs::path clean_path(m_bin_files.front());
    m_bin_files.pop();

    auto file = std::fopen(clean_path.string().c_str(), "rb");
    std::fseek(file, m_offest, SEEK_CUR); // skip the ordinal number
    std::fread(m_y_plane.data(), 1, m_y_plane.size(), file);
    std::fread(m_uv_plane.data(), 1, m_uv_plane.size(), file);
    fclose(file);

    auto y_plane = bnb::color_plane_vector(std::move(m_y_plane));
    auto uv_plane = bnb::color_plane_vector(std::move(m_uv_plane));

    image_format format;
    format.orientation = camera_orientation::deg_90; //TODO
    format.require_mirroring = false;
    format.width = m_width;
    format.height = m_height;

    bnb::yuv_image_t yuv(std::move(y_plane), std::move(uv_plane), format);
    bnb::full_image_t image(std::move(yuv));

    m_push_frame_cb(std::move(image));
}

void camera_bin_mocker::set_device_by_index(uint32_t)
{
    throw std::runtime_error("not implemented");
}

void camera_bin_mocker::set_device_by_id(const std::string&)
{
    throw std::runtime_error("not implemented");
}

void camera_bin_mocker::start()
{
    throw std::runtime_error("not implemented");
}
