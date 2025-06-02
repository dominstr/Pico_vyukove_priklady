cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

include(pico_sdk_import.cmake)

# FREERTOS: Set FreeRTOS Kernel path and include FreeRTOS
set(FREERTOS_KERNEL_PATH ${CMAKE_SOURCE_DIR}/../shared/FreeRTOS-Kernel)
include(${CMAKE_SOURCE_DIR}/../shared/freertos/FreeRTOS_Kernel_import.cmake)

project(freertos_setup C CXX ASM)

pico_sdk_init()

add_executable(freertos_setup freertos_setup.c)

pico_set_program_name(freertos_setup "freertos_setup")
pico_set_program_version(freertos_setup "0.1")

target_link_libraries(freertos_setup
    pico_stdlib
    FreeRTOS-Kernel-Heap4
    )

target_include_directories(freertos_setup PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../shared
    ${CMAKE_CURRENT_LIST_DIR}/../shared/freeRTOS
    ${FREERTOS_KERNEL_PATH}/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/include
    )

pico_add_extra_outputs(freertos_setup)

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(freertos_setup pico_cyw43_arch_none)
endif()
