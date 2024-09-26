/**
 * Copyright (C) 2021, A. Ridyard.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <stdio.h>

#include "nrf24_driver.h"
#include "pico/stdlib.h"

#include <tusb.h> // TinyUSB tud_cdc_connected()

int main(void)
{
  stdio_init_all();
  while (!tud_cdc_connected())
  {
    sleep_ms(10);
  }

  // GPIO pin numbers
  pin_manager_t my_pins = {
    .sck = 6,
    .copi = 7,
    .cipo = 4,
    .csn = 3,
    .ce = 2
  };

  nrf_manager_t my_config = {
    .channel = 80,
    .address_width = AW_5_BYTES,
    .dyn_payloads = DYNPD_ENABLE,
    .data_rate = RF_DR_250KBPS,
    .power = RF_PWR_NEG_12DBM,
    .retr_count = ARC_10RT,
    .retr_delay = ARD_500US
  };

  uint32_t my_baudrate = 5000000;
  nrf_client_t my_nrf;

  nrf_driver_create_client(&my_nrf);
  my_nrf.configure(&my_pins, my_baudrate);
  my_nrf.initialise(&my_config);

  my_nrf.rx_destination(DATA_PIPE_0, (uint8_t[]){0x31,0x30,0x30,0x30,0x30});
  my_nrf.rx_destination(DATA_PIPE_1, (uint8_t[]){0x31,0x30,0x30,0x30,0x30});
  my_nrf.rx_destination(DATA_PIPE_2, (uint8_t[]){0x32,0x30,0x30,0x30,0x30});
  my_nrf.rx_destination(DATA_PIPE_3, (uint8_t[]){0x33,0x30,0x30,0x30,0x30});

  my_nrf.receiver_mode();
  uint8_t pipe_number = 0;
  uint8_t payload[5];

  printf("Started\n");
  while (1)
  {
    if (my_nrf.is_packet(&pipe_number))
    {
      uint8_t size;
      my_nrf.read_packet_size(&size);
      if (size <=0 || size >=5) size = 2;
      my_nrf.read_packet(payload, size);
      printf("\nPacket received size %d (%x %x) on data pipe (%d)\n", size, payload[0], payload[1], pipe_number);
    }
  }
}
