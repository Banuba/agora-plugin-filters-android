cmake_minimum_required(VERSION 3.4.1)

project(native-lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(bnb_effect_player INTERFACE)
target_link_libraries(bnb_effect_player
        INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/lib/${ANDROID_ABI}/libbanuba.so
        )
target_include_directories(
        bnb_effect_player
        INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/bnb_sdk
)

add_library(yuv SHARED IMPORTED)
set_target_properties(yuv PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI}/libyuv_static.a)
include_directories(lib_yuv)

set(agora-lib ${PROJECT_SOURCE_DIR}/../jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libagora-rtc-sdk-jni.so)
link_libraries(${agora-lib})

include_directories(${CMAKE_CURRENT_LIST_DIR}/agora_rtc_kit)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/offscreen_effect_player)

add_library(
        plugin-lib

        SHARED

        filter/BanubaExtensionProvider.cpp
        filter/BanubaVideoFilter.cpp
        filter/BanubaVideoProcessor.cpp
        plugin_lib.cpp
)
target_include_directories(plugin-lib PRIVATE ${PROJECT_SOURCE_DIR})
target_link_libraries(
        plugin-lib
        android
        log
        offscreen_ep
        renderer
        yuv
        ${agora-lib}
)