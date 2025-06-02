cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(http_request C CXX ASM)

pico_sdk_init()

pico_add_library(example_lwip_http_util NOFLAG)
target_sources(example_lwip_http_util INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/example_http_client_util.c
        ${CMAKE_CURRENT_LIST_DIR}/../shared/cJSON/cJSON.c
)
pico_mirrored_target_link_libraries(example_lwip_http_util INTERFACE
        pico_lwip_http
        pico_lwip_mbedtls
        pico_mbedtls
)
target_include_directories(example_lwip_http_util INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(http_request http_request.c)

pico_set_program_name(http_request "http_request")
pico_set_program_version(http_request "0.1")

pico_enable_stdio_uart(http_request 0)
pico_enable_stdio_usb(http_request 1)

target_link_libraries(http_request
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        example_lwip_http_util
        )

target_include_directories(http_request PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared/picow_config
        ${CMAKE_CURRENT_LIST_DIR}/../shared
        ${CMAKE_CURRENT_LIST_DIR}/../shared/cJSON
)
if(PICO_CYW43_SUPPORTED)
        target_link_libraries(http_request pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(http_request)