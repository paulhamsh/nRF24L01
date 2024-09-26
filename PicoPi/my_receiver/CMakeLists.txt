add_executable(my_receiver my_receiver.c)

target_link_libraries(my_receiver
    PRIVATE
      nrf24_driver
      pico_stdlib
)

pico_enable_stdio_usb(my_receiver 1)
pico_enable_stdio_uart(my_receiver 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(my_receiver)
