set(include_dirs
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

file(GLOB_RECURSE srcs
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
        )

add_library(renderer STATIC ${srcs})

target_include_directories(renderer PUBLIC
        ${include_dirs}
        )

target_include_directories(renderer SYSTEM PUBLIC ${LIBYUV_INCLUDE_DIRS})

target_link_libraries(renderer
        GLESv3
        EGL
        ogl_utils
        yuv
        )