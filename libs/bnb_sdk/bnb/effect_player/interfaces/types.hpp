#pragma once

#include <bnb/effect_player/interfaces/all.hpp>
#include <memory>

namespace bnb::interfaces
{
    using camera_poi_listener_sptr = std::shared_ptr<camera_poi_listener>;
    using camera_poi_listener_wptr = std::weak_ptr<camera_poi_listener>;
    using camera_poi_listener_uptr = std::unique_ptr<camera_poi_listener>;
    using camera_poi_listener_ptr = bnb::interfaces::camera_poi_listener*;

    using effect_sptr = std::shared_ptr<effect>;
    using effect_wptr = std::weak_ptr<effect>;
    using effect_uptr = std::unique_ptr<effect>;
    using effect_ptr = bnb::interfaces::effect*;

    using effect_activated_listener_sptr = std::shared_ptr<effect_activated_listener>;
    using effect_activated_listener_wptr = std::weak_ptr<effect_activated_listener>;
    using effect_activated_listener_uptr = std::unique_ptr<effect_activated_listener>;
    using effect_activated_listener_ptr = bnb::interfaces::effect_activated_listener*;

    using effect_activation_completion_listener_sptr = std::shared_ptr<effect_activation_completion_listener>;
    using effect_activation_completion_listener_wptr = std::weak_ptr<effect_activation_completion_listener>;
    using effect_activation_completion_listener_uptr = std::unique_ptr<effect_activation_completion_listener>;
    using effect_activation_completion_listener_ptr = bnb::interfaces::effect_activation_completion_listener*;

    using effect_event_listener_sptr = std::shared_ptr<effect_event_listener>;
    using effect_event_listener_wptr = std::weak_ptr<effect_event_listener>;
    using effect_event_listener_uptr = std::unique_ptr<effect_event_listener>;
    using effect_event_listener_ptr = bnb::interfaces::effect_event_listener*;

    using effect_info_listener_sptr = std::shared_ptr<effect_info_listener>;
    using effect_info_listener_wptr = std::weak_ptr<effect_info_listener>;
    using effect_info_listener_uptr = std::unique_ptr<effect_info_listener>;
    using effect_info_listener_ptr = bnb::interfaces::effect_info_listener*;

    using effect_manager_sptr = std::shared_ptr<effect_manager>;
    using effect_manager_wptr = std::weak_ptr<effect_manager>;
    using effect_manager_uptr = std::unique_ptr<effect_manager>;
    using effect_manager_ptr = bnb::interfaces::effect_manager*;

    using effect_player_sptr = std::shared_ptr<effect_player>;
    using effect_player_wptr = std::weak_ptr<effect_player>;
    using effect_player_uptr = std::unique_ptr<effect_player>;
    using effect_player_ptr = bnb::interfaces::effect_player*;

    using effect_player_configuration_sptr = std::shared_ptr<effect_player_configuration>;
    using effect_player_configuration_wptr = std::weak_ptr<effect_player_configuration>;
    using effect_player_configuration_uptr = std::unique_ptr<effect_player_configuration>;
    using effect_player_configuration_ptr = bnb::interfaces::effect_player_configuration*;

    using error_listener_sptr = std::shared_ptr<error_listener>;
    using error_listener_wptr = std::weak_ptr<error_listener>;
    using error_listener_uptr = std::unique_ptr<error_listener>;
    using error_listener_ptr = bnb::interfaces::error_listener*;

    using face_number_listener_sptr = std::shared_ptr<face_number_listener>;
    using face_number_listener_wptr = std::weak_ptr<face_number_listener>;
    using face_number_listener_uptr = std::unique_ptr<face_number_listener>;
    using face_number_listener_ptr = bnb::interfaces::face_number_listener*;

    using frame_data_listener_sptr = std::shared_ptr<frame_data_listener>;
    using frame_data_listener_wptr = std::weak_ptr<frame_data_listener>;
    using frame_data_listener_uptr = std::unique_ptr<frame_data_listener>;
    using frame_data_listener_ptr = bnb::interfaces::frame_data_listener*;

    using frame_duration_listener_sptr = std::shared_ptr<frame_duration_listener>;
    using frame_duration_listener_wptr = std::weak_ptr<frame_duration_listener>;
    using frame_duration_listener_uptr = std::unique_ptr<frame_duration_listener>;
    using frame_duration_listener_ptr = bnb::interfaces::frame_duration_listener*;

    using frame_processor_sptr = std::shared_ptr<frame_processor>;
    using frame_processor_wptr = std::weak_ptr<frame_processor>;
    using frame_processor_uptr = std::unique_ptr<frame_processor>;
    using frame_processor_ptr = bnb::interfaces::frame_processor*;

    using hint_listener_sptr = std::shared_ptr<hint_listener>;
    using hint_listener_wptr = std::weak_ptr<hint_listener>;
    using hint_listener_uptr = std::unique_ptr<hint_listener>;
    using hint_listener_ptr = bnb::interfaces::hint_listener*;

    using input_manager_sptr = std::shared_ptr<input_manager>;
    using input_manager_wptr = std::weak_ptr<input_manager>;
    using input_manager_uptr = std::unique_ptr<input_manager>;
    using input_manager_ptr = bnb::interfaces::input_manager*;

    using js_callback_sptr = std::shared_ptr<js_callback>;
    using js_callback_wptr = std::weak_ptr<js_callback>;
    using js_callback_uptr = std::unique_ptr<js_callback>;
    using js_callback_ptr = bnb::interfaces::js_callback*;

    using low_light_listener_sptr = std::shared_ptr<low_light_listener>;
    using low_light_listener_wptr = std::weak_ptr<low_light_listener>;
    using low_light_listener_uptr = std::unique_ptr<low_light_listener>;
    using low_light_listener_ptr = bnb::interfaces::low_light_listener*;

    using processor_configuration_sptr = std::shared_ptr<processor_configuration>;
    using processor_configuration_wptr = std::weak_ptr<processor_configuration>;
    using processor_configuration_uptr = std::unique_ptr<processor_configuration>;
    using processor_configuration_ptr = bnb::interfaces::processor_configuration*;

}
