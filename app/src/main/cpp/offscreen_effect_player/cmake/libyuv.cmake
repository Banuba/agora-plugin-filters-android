cmake_minimum_required(VERSION 3.5)

include(ExternalProject)

if(ANDROID)
  set(LIBYUV_ROOT ${CMAKE_CURRENT_BINARY_DIR}/libyuv-prefix/src/libyuv-build)
  set(LIBYUV_MK ${CMAKE_CURRENT_BINARY_DIR}/libyuv-prefix/src/libyuv)

  ExternalProject_Add(
    libyuv
    GIT_REPOSITORY https://chromium.googlesource.com/libyuv/libyuv
    GIT_TAG 312c02a5aad4adda67cb2e0cc93a497d12845522
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/libyuv/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt && ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/libyuv/Android.mk <SOURCE_DIR>/Android.mk
    BUILD_COMMAND
      ${ANDROID_NDK}/ndk-build
      -C ${LIBYUV_ROOT}
      NDK_PROJECT_PATH=null
      APP_BUILD_SCRIPT=${LIBYUV_MK}/Android.mk
      NDK_OUT=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/../..
      NDK_LIBS_OUT=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/..
      APP_ABI=${ANDROID_ABI}
      NDK_ALL_ABIS=${ANDROID_ABI}
      APP_PLATFORM=${ANDROID_PLATFORM}
    BUILD_BYPRODUCTS ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libyuv.so
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)
else()
  ExternalProject_Add(
    libyuv
    GIT_REPOSITORY https://chromium.googlesource.com/libyuv/libyuv
    GIT_TAG 312c02a5aad4adda67cb2e0cc93a497d12845522
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
    PATCH_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_LIST_DIR}/libyuv/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)
endif()

ExternalProject_Get_Property(libyuv source_dir)
set(LIBYUV_INCLUDE_DIRS ${source_dir}/include)

if(NOT ANDROID)
  ExternalProject_Get_Property(libyuv binary_dir)
  set(LIBYUV_LIBRARY_PATH ${binary_dir})
endif()

set(LIBYUV_LIBRARY yuv)
add_library(${LIBYUV_LIBRARY} UNKNOWN IMPORTED)
if(CMAKE_GENERATOR STREQUAL Xcode)
  set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
    IMPORTED_LOCATION ${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}yuv.a
    IMPORTED_LOCATION_RELEASE ${LIBYUV_LIBRARY_PATH}/Release/${CMAKE_FIND_LIBRARY_PREFIXES}yuv.a
    IMPORTED_LOCATION_DEBUG ${LIBYUV_LIBRARY_PATH}/Debug/${CMAKE_FIND_LIBRARY_PREFIXES}yuv.a
    )
elseif(MSVC)
  set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
    IMPORTED_LOCATION_DEBUG "${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}/Debug/yuv.lib"
    IMPORTED_LOCATION_RELEASE "${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}/Release/yuv.lib"
  )
elseif(ANDROID)
  set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
    IMPORTED_LOCATION_DEBUG ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libyuv.so
    IMPORTED_LOCATION_RELEASE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libyuv.so
    IMPORTED_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libyuv.so
  )
else()
  set_target_properties(${LIBYUV_LIBRARY} PROPERTIES
    IMPORTED_LOCATION ${LIBYUV_LIBRARY_PATH}/${CMAKE_FIND_LIBRARY_PREFIXES}libyuv.a
  )
endif()

add_dependencies(${LIBYUV_LIBRARY} libyuv)