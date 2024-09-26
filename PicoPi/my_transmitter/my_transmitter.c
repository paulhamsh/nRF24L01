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
 *
 * @file primary_transmitter.c
 *
 * @brief example of NRF24L01 setup as a primary transmitter using the
 * NRF24L01 driver. Different data types and sizes are sent to different
 * receiver data pipes as an example, utilizing dynamic payload size.
 */

#include <stdio.h>

#include "nrf24_driver.h"
#include "pico/stdlib.h"

#include <tusb.h> // TinyUSB tud_cdc_connected()

int main(void)
{
  stdio_init_all();

  // wait until the CDC ACM (serial port emulation) is connected
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
  my_nrf.standby_mode();

  //typedef struct payload_t { uint8_t one; uint8_t two; } payload_t;
  //payload_t payload = { .one = 0x11, .two = 0x22 };
  uint8_t payload[5] = {0x11, 0x22, 0x00, 0x00, 0x00};


  fn_status_t success = 0;


  while (1) {
    // send to receiver's DATA_PIPE_0 address
    my_nrf.tx_destination((uint8_t[]) {0x31,0x30,0x30,0x30,0x30});
    success = my_nrf.send_packet(payload, 2);

    if (success)
      //printf("\nPacket sent:- 0x%x 0x%x\n", payload.one, payload.two);
      printf("\nPacket sent:- 0x%x 0x%x\n", payload[0], payload[1]);
    else
      printf("\nPacket not sent:- Receiver not available.\n");

    //payload.two++;
    payload[1]++;

    sleep_ms(3000);
  }
}
