cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(blink_from_example C CXX ASM)

pico_sdk_init()

add_executable(blink_from_example
    blink.c
    )

target_link_libraries(blink_from_example pico_stdlib)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink_from_example pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(blink_from_example)