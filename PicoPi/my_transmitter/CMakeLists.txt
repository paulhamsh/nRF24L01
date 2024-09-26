add_executable(my_transmitter my_transmitter.c)

target_link_libraries(my_transmitter
    PRIVATE
      nrf24_driver
      pico_stdlib
)

pico_enable_stdio_usb(my_transmitter 1)
pico_enable_stdio_uart(my_transmitter 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(my_transmitter)
