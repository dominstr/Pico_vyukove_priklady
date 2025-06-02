cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(hello_adc_from_example C CXX ASM)

pico_sdk_init()

add_executable(hello_adc_from_example
        hello_adc.c
        )

pico_enable_stdio_uart(hello_adc_from_example 0)
pico_enable_stdio_usb(hello_adc_from_example 1)

target_link_libraries(hello_adc_from_example pico_stdlib hardware_adc)

target_include_directories(hello_adc_from_example PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)

pico_add_extra_outputs(hello_adc_from_example)