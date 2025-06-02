cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(hello_st7735 C CXX ASM)

pico_sdk_init()

add_executable(hello_st7735 hello_st7735.cpp
  ../shared/displaylib/src/displaylib_16/st7735.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_graphics.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_Print.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_Font.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_common.cpp
  ../shared/displaylib/src/displaylib_16/Bitmap_TEST_Data.cpp
)

target_compile_options(hello_st7735 PUBLIC "-std=c++20")

pico_set_program_name(hello_st7735 "hello_st7735")
pico_set_program_version(hello_st7735 "0.1")

pico_enable_stdio_uart(hello_st7735 0)
pico_enable_stdio_usb(hello_st7735 1)

target_include_directories(hello_st7735 PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../shared/displaylib/include
    ${CMAKE_CURRENT_LIST_DIR}/../shared
)

target_link_libraries(hello_st7735
    pico_stdlib
    hardware_spi
)

pico_add_extra_outputs(hello_st7735)