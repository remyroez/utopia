# cmake
cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0076 NEW)

# vcpkg
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
endif()

# project
project(utopia C CXX)

# executable
add_executable(main WIN32 MACOSX_BUNDLE)
set_target_properties(main PROPERTIES OUTPUT_NAME utopia)
target_compile_features(main PRIVATE cxx_std_20)
target_compile_definitions(main PRIVATE $<$<CONFIG:Debug>:DEBUG_MODE>)
target_compile_options(main PRIVATE /wd4324)

# dependencies
find_package(argparse CONFIG REQUIRED)
target_link_libraries(main PRIVATE argparse::argparse)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE SDL3::SDL3)

#find_package(SDL3_image CONFIG REQUIRED)
#target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,SDL3_image::SDL3_image-shared,SDL3_image::SDL3_image-static>)

#find_package(SDL3_ttf CONFIG REQUIRED)
#target_link_libraries(main PRIVATE $<IF:$<TARGET_EXISTS:SDL3_ttf::SDL3_ttf>,SDL3_ttf::SDL3_ttf,SDL3_ttf::SDL3_ttf-static>)     

find_package(imgui CONFIG REQUIRED)
target_link_libraries(main PRIVATE imgui::imgui)

# msvc
if (MSVC)
  target_compile_options(main PRIVATE /W4)
  set_target_properties(main PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
  add_definitions(/bigobj)
else()
  target_compile_options(main PRIVATE -Wall -Wextra)
endif()

# src
add_subdirectory(src)

target_include_directories(main PRIVATE src)

get_property("TARGET_SOURCE_FILES" TARGET main PROPERTY SOURCES)
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${TARGET_SOURCE_FILES})
