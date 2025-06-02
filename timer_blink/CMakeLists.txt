cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(timer_blink C CXX ASM)

pico_sdk_init()

add_executable(timer_blink timer_blink.c )

pico_set_program_name(timer_blink "timer_blink")
pico_set_program_version(timer_blink "0.1")

pico_enable_stdio_uart(timer_blink 0)
pico_enable_stdio_usb(timer_blink 1)

target_link_libraries(timer_blink
        pico_stdlib
        hardware_timer
        )

target_include_directories(timer_blink PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(timer_blink pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(timer_blink)