cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(buzzer C CXX ASM)

pico_sdk_init()

add_executable(buzzer buzzer.c )

pico_set_program_name(buzzer "buzzer")
pico_set_program_version(buzzer "0.1")

target_link_libraries(buzzer
        pico_stdlib
        hardware_pwm
        )

target_include_directories(buzzer PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)

pico_add_extra_outputs(buzzer)