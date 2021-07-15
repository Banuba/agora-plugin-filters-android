#include <bnb/spal/camera/cbor_mocker.hpp>

#include <bnb/utils/std/filesystem.hpp>

using namespace bnb;
using namespace std::literals;

camera_cbor_mocker::camera_cbor_mocker(
    const camera_base::push_frame_cb_t& cb, const std::string& path)
    : camera_base_mocker(cb)
    , m_decoder(path)
{
    if (fs::path(path).extension() != ".cbor") {
        throw std::runtime_error(
            "Wrong file format is provided: "s + fs::path(path).extension().string()
            + "; expected: .cbor"s);
    }

    start();
}

void camera_cbor_mocker::update_image_content()
{
    if (m_decoder.next(&fd)) {
        using namespace std::literals;
        std::this_thread::sleep_for(50ms);
        auto& full_image = const_cast<full_image_t&>(fd.get_data<full_image_t>());
        m_push_frame_cb(std::move(full_image));
    } else {
        abort();
    }
}
