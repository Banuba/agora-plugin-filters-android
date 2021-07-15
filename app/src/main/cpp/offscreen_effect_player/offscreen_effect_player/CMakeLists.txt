set(include_dirs
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

file(GLOB_RECURSE srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        )

add_library(offscreen_ep STATIC ${srcs})

target_include_directories(offscreen_ep PUBLIC
        ${include_dirs}
        )

target_include_directories(offscreen_ep SYSTEM PUBLIC ${LIBYUV_INCLUDE_DIRS})
target_include_directories(offscreen_ep PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(offscreen_ep
        offscreen_rt
        utils
        yuv
        bnb_effect_player
        )