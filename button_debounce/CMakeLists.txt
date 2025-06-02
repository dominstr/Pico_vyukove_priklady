cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(button_debounce C CXX ASM)

pico_sdk_init()

add_executable(button_debounce button_debounce.c )

pico_set_program_name(button_debounce "button_debounce")
pico_set_program_version(button_debounce "0.1")

pico_enable_stdio_uart(button_debounce 0)
pico_enable_stdio_usb(button_debounce 1)

target_link_libraries(button_debounce
        pico_stdlib)

target_include_directories(button_debounce PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(button_debounce pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(button_debounce)