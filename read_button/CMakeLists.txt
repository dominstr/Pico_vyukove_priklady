cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(read_button C CXX ASM)

pico_sdk_init()

add_executable(read_button read_button.c )

pico_set_program_name(read_button "read_button")
pico_set_program_version(read_button "0.1")

pico_enable_stdio_uart(read_button 0)
pico_enable_stdio_usb(read_button 1)

target_link_libraries(read_button
        pico_stdlib)

target_include_directories(read_button PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(read_button pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(read_button)