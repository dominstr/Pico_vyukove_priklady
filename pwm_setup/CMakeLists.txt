cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(pwm_setup C CXX ASM)

pico_sdk_init()

add_executable(pwm_setup pwm_setup.c )

pico_set_program_name(pwm_setup "pwm_setup")
pico_set_program_version(pwm_setup "0.1")

pico_enable_stdio_uart(pwm_setup 0)
pico_enable_stdio_usb(pwm_setup 1)

target_link_libraries(pwm_setup
        pico_stdlib
        hardware_pwm
        )

target_include_directories(pwm_setup PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
        )

pico_add_extra_outputs(pwm_setup)