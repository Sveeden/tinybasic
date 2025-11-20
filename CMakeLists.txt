cmake_minimum_required(VERSION 3.13)

# Set board to pico2 for Raspberry Pi Pico 2
set(PICO_BOARD pico2)

include(pico_sdk_import.cmake)

project(obi88basic C CXX)
pico_sdk_init()

add_executable(obi88basic main.c token.c execute.c variables.c program.c loops.c filesystem.c math.c help.c)
target_link_libraries(obi88basic pico_stdlib hardware_flash hardware_sync m)
pico_enable_stdio_usb(obi88basic 1)
pico_enable_stdio_uart(obi88basic 0)
pico_add_extra_outputs(obi88basic)