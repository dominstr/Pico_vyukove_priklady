cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(weather_station C CXX ASM)

pico_sdk_init()

pico_add_library(example_lwip_http_util NOFLAG)
target_sources(example_lwip_http_util INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/example_http_client_util.c
        ${CMAKE_CURRENT_LIST_DIR}/../shared/cJSON/cJSON.c
)
pico_mirrored_target_link_libraries(example_lwip_http_util INTERFACE
        pico_lwip_http
        pico_lwip_mbedtls
        pico_mbedtls
)
target_include_directories(example_lwip_http_util INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(weather_station weather_station.cpp
  ../shared/displaylib/src/displaylib_16/st7735.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_graphics.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_Print.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_Font.cpp
  ../shared/displaylib/src/displaylib_16/displaylib_16_common.cpp
  ../shared/displaylib/src/displaylib_16/Bitmap_TEST_Data.cpp
)

target_compile_options(weather_station PUBLIC "-std=c++20")

pico_set_program_name(weather_station "weather_station")
pico_set_program_version(weather_station "0.1")

pico_enable_stdio_uart(weather_station 0)
pico_enable_stdio_usb(weather_station 1)

target_link_libraries(weather_station
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        example_lwip_http_util
        hardware_spi
        )

target_include_directories(weather_station PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared/picow_config
        ${CMAKE_CURRENT_LIST_DIR}/../shared
        ${CMAKE_CURRENT_LIST_DIR}/../shared/cJSON
        ${CMAKE_CURRENT_LIST_DIR}/../shared/displaylib/include
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)
if(PICO_CYW43_SUPPORTED)
        target_link_libraries(weather_station pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(weather_station)