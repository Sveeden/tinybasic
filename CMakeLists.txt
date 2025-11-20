cmake_minimum_required(VERSION 3.13)

# Set board to pico2 for Raspberry Pi Pico 2
set(PICO_BOARD pico2)

include(pico_sdk_import.cmake)

project(tinybasic C CXX)
pico_sdk_init()

add_executable(tinybasic main.c token.c execute.c variables.c program.c loops.c filesystem.c)
target_link_libraries(tinybasic pico_stdlib hardware_flash hardware_sync)
pico_enable_stdio_usb(tinybasic 1)
pico_enable_stdio_uart(tinybasic 0)
pico_add_extra_outputs(tinybasic)