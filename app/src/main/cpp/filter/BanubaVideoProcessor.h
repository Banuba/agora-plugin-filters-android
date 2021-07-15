#pragma once

#include <string>
#include <AgoraRefPtr.h>
#include <NGIAgoraExtensionControl.h>
#include <AgoraMediaBase.h>
#include <offscreen_effect_player.hpp>
#include <offscreen_render_target.hpp>
#include <render_thread.hpp>

namespace agora::extension {

    class BanubaVideoProcessor : public RefCountInterface {

    public:

        BanubaVideoProcessor(std::vector<std::string> path_to_resources,
                             std::string client_token,
                             int32_t width,
                             int32_t height);

        void process_frame(const agora::media::base::VideoFrame &capturedFrame);

        void set_parameter(const std::string &key, const std::string &parameter);

        int set_extension_control(agora::rtc::IExtensionControl *control) {
            m_control = control;
            return 0;
        };

        int set_extension_vendor(const char *id) {
            int len = std::string(id).length() + 1;
            m_id = static_cast<char *>(malloc(len));
            memset(m_id, 0, len);
            strcpy(m_id, id);
            return 0;
        };

    protected:
        ~BanubaVideoProcessor() = default;

    private:
        void send_event(const char *key, const char *data);

        const std::vector<std::string> m_path_to_resources;
        const std::string m_client_token;

        agora::rtc::IExtensionControl *m_control;
        char *m_id;

        ioep_sptr m_oep;

        bnb::image_format image_format;
        std::vector<std::uint8_t> y_plane;
        std::vector<std::uint8_t> uv_plane;
        bnb::interfaces::orient_format target_orient{bnb::camera_orientation::deg_0, false};
    };
}