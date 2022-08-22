
add_library(hell_chars hell_chars.c)
add_library(osc_screen osc_screen.c)
add_library(demo_functions demo_functions.c)
add_executable(hellschreiber hellschreiber.c)

# Create C header file with the name <pio program>.pio.h
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/verticaldef.pio)
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/readfbzmod.pio)
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/hello.pio)


pico_enable_stdio_usb(hellschreiber 1)
pico_enable_stdio_uart(hellschreiber 1)
pico_add_extra_outputs(hellschreiber)
target_link_libraries(osc_screen pico_stdlib)
target_link_libraries(demo_functions osc_screen)
target_link_libraries(${PROJECT_NAME}
    osc_screen
    pico_stdlib
    pico_multicore
    hell_chars
    demo_functions
    hardware_pio
)