cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(relay C CXX ASM)

pico_sdk_init()

add_executable(relay relay.c )

pico_set_program_name(relay "relay")
pico_set_program_version(relay "0.1")

pico_enable_stdio_uart(relay 0)
pico_enable_stdio_usb(relay 0)

target_link_libraries(relay
        pico_stdlib)

target_include_directories(relay PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)

pico_add_extra_outputs(relay)