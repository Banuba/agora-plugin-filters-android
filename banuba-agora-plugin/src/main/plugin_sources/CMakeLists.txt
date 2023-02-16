cmake_minimum_required(VERSION 3.9)

project(native-lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(banuba-lib bnb_effect_player)
add_library(${banuba-lib} INTERFACE)
target_link_libraries(${banuba-lib} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/lib/${ANDROID_ABI}/libbanuba.so
)
target_include_directories(${banuba-lib} INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../../../../libs/bnb_sdk
)

set(agora-lib agora_rtc_sdk)
add_library(${agora-lib} INTERFACE)
target_link_libraries(${agora-lib} INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../../../../libs/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libagora-rtc-sdk.so
)
target_include_directories(${agora-lib} INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../../../../libs/agora_rtc_sdk/include
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/oep)

add_library(banuba-plugin SHARED
        filter/BanubaExtensionProvider.cpp
        filter/BanubaExtensionProvider.h
        filter/BanubaVideoFilter.cpp
        filter/BanubaVideoFilter.h
        filter/BanubaVideoProcessor.cpp
        filter/BanubaVideoProcessor.h
        effect_player.cpp
        effect_player.hpp
        render_context.cpp
        render_context.hpp
        plugin_lib.cpp
)
target_include_directories(banuba-plugin PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(banuba-plugin
        # OEP submodule targets
        bnb_oep_offscreen_effect_player_target
        bnb_oep_offscreen_render_target_target
        bnb_oep_image_processing_result_target
        bnb_oep_pixel_buffer_target
        # external libraries
        ${banuba-lib}
        ${agora-lib}
        # system libraries
        EGL
)
