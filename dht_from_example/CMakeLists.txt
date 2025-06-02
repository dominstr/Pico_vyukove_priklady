cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(dht_from_example C CXX ASM)

pico_sdk_init()

add_executable(dht_from_example
        dht.c
        )

target_include_directories(dht_from_example PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)

target_link_libraries(dht_from_example pico_stdlib)

pico_enable_stdio_uart(dht_from_example 0)
pico_enable_stdio_usb(dht_from_example 1)

pico_add_extra_outputs(dht_from_example)