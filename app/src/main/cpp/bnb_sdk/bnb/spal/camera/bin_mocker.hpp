#pragma once

#include <bnb/types/base_types.hpp>
#include <bnb/spal/camera/base_mocker.hpp>

#include <thread>
#include <string>
#include <queue>

namespace bnb
{
    class BNB_EXPORT camera_bin_mocker final : public camera_base_mocker
    {
    public:
        camera_bin_mocker(const camera_base::push_frame_cb_t& cb, const std::string& input_path, size_t width, size_t height, size_t an_offset);
        ~camera_bin_mocker() final = default;

        void set_device_by_index(uint32_t index) override;
        void set_device_by_id(const std::string& device_id) override;
        void start() override;

    protected:
        void update_image_content() final;

    private:
        std::queue<std::string> m_bin_files;
        const size_t m_width, m_height;
        const size_t m_offest;
    };

} // namespace bnb
