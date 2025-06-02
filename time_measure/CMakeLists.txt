cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(time_measure C CXX ASM)

pico_sdk_init()

add_executable(time_measure time_measure.c )

pico_set_program_name(time_measure "time_measure")
pico_set_program_version(time_measure "0.1")

pico_enable_stdio_uart(time_measure 0)
pico_enable_stdio_usb(time_measure 1)

target_link_libraries(time_measure
        pico_stdlib)

target_include_directories(time_measure PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)

target_link_libraries(time_measure 
        hardware_gpio
        hardware_timer
        )

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(time_measure pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(time_measure)