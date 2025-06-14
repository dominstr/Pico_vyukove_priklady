cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(blink_traffic_lights C CXX ASM)

pico_sdk_init()

add_executable(blink_traffic_lights blink_traffic_lights.c )

pico_set_program_name(blink_traffic_lights "blink_traffic_lights")
pico_set_program_version(blink_traffic_lights "0.1")

pico_enable_stdio_uart(blink_traffic_lights 0)
pico_enable_stdio_usb(blink_traffic_lights 1)

target_link_libraries(blink_traffic_lights
        pico_stdlib)

target_include_directories(blink_traffic_lights PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink_traffic_lights pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(blink_traffic_lights)

