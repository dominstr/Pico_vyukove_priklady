cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

project(potentiometer_brightness C CXX ASM)

pico_sdk_init()

add_executable(potentiometer_brightness potentiometer_brightness.c )

pico_set_program_name(potentiometer_brightness "potentiometer_brightness")
pico_set_program_version(potentiometer_brightness "0.1")

pico_enable_stdio_uart(potentiometer_brightness 0)
pico_enable_stdio_usb(potentiometer_brightness 1)

target_link_libraries(potentiometer_brightness
        pico_stdlib
        hardware_pwm
        hardware_adc
        )

target_include_directories(potentiometer_brightness PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/../shared
)

pico_add_extra_outputs(potentiometer_brightness)