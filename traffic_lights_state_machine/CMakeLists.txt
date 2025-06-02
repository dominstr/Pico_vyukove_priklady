cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(traffic_lights_state_machine C CXX ASM)

pico_sdk_init()

add_executable(traffic_lights_state_machine traffic_lights_state_machine.c )

pico_set_program_name(traffic_lights_state_machine "traffic_lights_state_machine")
pico_set_program_version(traffic_lights_state_machine "0.1")

pico_enable_stdio_uart(traffic_lights_state_machine 0)
pico_enable_stdio_usb(traffic_lights_state_machine 1)

target_link_libraries(traffic_lights_state_machine
        pico_stdlib)

target_include_directories(traffic_lights_state_machine PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/../shared
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(traffic_lights_state_machine pico_cyw43_arch_none)
endif()

pico_add_extra_outputs(traffic_lights_state_machine)