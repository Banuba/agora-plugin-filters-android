#pragma once

#include <string>
#include <AgoraRefPtr.h>
#include <NGIAgoraExtensionControl.h>
#include <AgoraMediaBase.h>
#include <interfaces/offscreen_effect_player.hpp>

namespace agora::extension
{

    class BanubaVideoProcessor : public RefCountInterface
    {
    public:
        BanubaVideoProcessor() = default;

        void process_frame(const agora_refptr<rtc::IVideoFrame>& frame);

        void set_parameter(const std::string& key, const std::string& parameter);

        int set_extension_control(agora::agora_refptr<rtc::IExtensionVideoFilter::Control> control);

    protected:
        ~BanubaVideoProcessor() = default;

    private:
        void initialize();

        void create();

        void destroy();

        void update_oep_surface_size(int32_t width, int32_t height);

    private:
        std::string m_path_to_resources;
        std::string m_path_to_effects;
        std::string m_client_token;
        std::atomic<bnb::oep::interfaces::rotation> m_oep_input_rotation = bnb::oep::interfaces::rotation::deg0;

        agora::agora_refptr<rtc::IExtensionVideoFilter::Control> m_control;
        std::atomic_bool m_is_effect_loaded = false;
        offscreen_effect_player_sptr m_oep;
        int32_t m_oep_surface_width = 0;
        int32_t m_oep_surface_height = 0;
    }; /* class BanubaVideoProcessor */


    /* BanubaVideoProcessor::set_extension_control */
    inline int BanubaVideoProcessor::set_extension_control(agora::agora_refptr<rtc::IExtensionVideoFilter::Control> control)
    {
        m_control = control;
        return 0;
    };
} // namespace agora::extension
