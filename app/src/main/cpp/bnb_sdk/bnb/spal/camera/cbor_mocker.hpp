#pragma once

#include <bnb/types/base_types.hpp>
#include <bnb/spal/camera/base_mocker.hpp>
#include <bnb/spal/serialization/frame_data_deserializer.hpp>

#include <thread>
#include <string>

namespace bnb
{
    class camera_cbor_mocker final : public camera_base_mocker
    {
    public:
        camera_cbor_mocker(const camera_base::push_frame_cb_t& cb, const std::string& input_path);
        ~camera_cbor_mocker() final = default;

    protected:
        void update_image_content() final;

    private:
        serialization::frame_data_deserializer m_decoder;
        frame_data fd;
    };

} // namespace bnb
