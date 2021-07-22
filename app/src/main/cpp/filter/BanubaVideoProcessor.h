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

        BanubaVideoProcessor();

        void process_frame(const agora_refptr<rtc::IVideoFrame> &in);

        void set_parameter(const std::string &key, const std::string &parameter);

        int set_extension_control(
                agora::agora_refptr<rtc::IExtensionVideoFilter::Control> control
        ) {
            m_control = control;
            return 0;
        };

    protected:
        ~BanubaVideoProcessor() = default;

    private:
        void send_event(const char *key, const char *data);

        void initialize();
        void create_ep(int32_t width, int32_t height);

        std::string m_path_to_resources;
        std::string m_client_token;
        bool m_is_initialized = false;

        agora::agora_refptr<rtc::IExtensionVideoFilter::Control> m_control;
        ioep_sptr m_oep;

        rtc::VideoFrameData m_captured_frame;
        bnb::image_format m_image_format;
        bnb::interfaces::orient_format m_target_orient{bnb::camera_orientation::deg_0, false};
    };
}