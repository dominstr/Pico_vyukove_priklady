cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(response C CXX ASM)

pico_sdk_init()

add_executable(response response.c )

pico_set_program_name(response "response")
pico_set_program_version(response "0.1")

pico_enable_stdio_uart(response 0)
pico_enable_stdio_usb(response 1)

target_link_libraries(response
        pico_stdlib)

target_include_directories(response PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(response pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(response)