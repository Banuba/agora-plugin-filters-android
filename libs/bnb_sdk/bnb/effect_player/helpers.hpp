#pragma once

#include <bnb/effect_player/interfaces/camera_poi_listener.hpp>
#include <bnb/effect_player/interfaces/effect_event_listener.hpp>
#include <bnb/effect_player/interfaces/error_listener.hpp>
#include <bnb/effect_player/interfaces/face_number_listener.hpp>
#include <bnb/effect_player/interfaces/frame_duration_listener.hpp>
#include <bnb/effect_player/interfaces/hint_listener.hpp>
#include <bnb/effect_player/interfaces/low_light_listener.hpp>
#include <bnb/effect_player/interfaces/frame_data_listener.hpp>
#include <bnb/effect_player/interfaces/effect_activated_listener.hpp>
#include <bnb/effect_player/interfaces/effect_info_listener.hpp>
#include <bnb/effect_player/interfaces/effect_info.hpp>

#include <functional>
#include <memory>

namespace bnb
{
    //
    // Convenient C++ methods for creating interface implementations from std::functions
    //

    using error_listener_callback = std::function<void(const std::string&, const std::string&)>;
    using camera_poi_callback = std::function<void(float x, float y)>;
    using effect_event_callback = std::function<void(
        const std::string&, const std::unordered_map<std::string, std::string>&
    )>;
    using face_number_callback = std::function<void(int32_t)>;
    using frame_duration_callback = std::function<void(float, float)>;
    using hint_callback = std::function<void(const std::string&)>;
    using low_light_callback = std::function<void(bool)>;
    using frame_data_callback = std::function<void(const std::shared_ptr<interfaces::frame_data>&)>;
    using effect_activated_callback = std::function<void(const std::string&)>;
    using effect_info_callback = std::function<void(const interfaces::effect_info&)>;

    std::shared_ptr<interfaces::error_listener>
    make_error_listener(const error_listener_callback& callback);

    std::shared_ptr<interfaces::camera_poi_listener>
    make_camera_poi_listener(const camera_poi_callback& callback);

    std::shared_ptr<interfaces::effect_event_listener>
    make_effect_event_listener(const effect_event_callback& callback);

    std::shared_ptr<interfaces::face_number_listener>
    make_face_number_listener(const face_number_callback& callback);

    std::shared_ptr<interfaces::frame_duration_listener> make_frame_duration_listener(
        const frame_duration_callback& recognizer_callback,
        const frame_duration_callback& camera_callback,
        const frame_duration_callback& render_callback
    );

    std::shared_ptr<interfaces::hint_listener> make_hint_listener(const hint_callback& callback);

    std::shared_ptr<interfaces::frame_data_listener>
    make_frame_data_listener(const frame_data_callback& callback);

    std::shared_ptr<interfaces::effect_activated_listener>
    make_effect_activated_listener(const effect_activated_callback& callback);

    std::shared_ptr<interfaces::effect_info_listener>
    make_effect_info_listener(const effect_info_callback& callback);
} // namespace bnb
