cmake_minimum_required(VERSION 4.2)
project(doom_renderer LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory("external/SDL3")

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_executable(${PROJECT_NAME} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)

