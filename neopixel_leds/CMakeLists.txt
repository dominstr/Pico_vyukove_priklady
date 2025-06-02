cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(neopixel_leds C CXX ASM)

pico_sdk_init()

add_executable(neopixel_leds
        neopixel_leds.c
        ws2812.pio
        )
        
pico_set_program_name(neopixel_leds "neopixel_leds")
pico_set_program_version(neopixel_leds "0.1")

pico_enable_stdio_uart(neopixel_leds 0)
pico_enable_stdio_usb(neopixel_leds 1)

target_link_libraries(neopixel_leds
        pico_stdlib
        hardware_pio
        hardware_clocks
        )

target_include_directories(neopixel_leds PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)

pico_add_extra_outputs(neopixel_leds)