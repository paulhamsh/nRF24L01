/**
 * Based on transmitter.c copyright (C) 2021, A. Ridyard.
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

#include <tusb.h> 

int main(void)
{
  stdio_init_all();

  // wait until the CDC ACM (serial port emulation) is connected
  while (!tud_cdc_connected()) {
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
  
  typedef struct payload_t { uint8_t one; uint8_t two; } payload_t;
  payload_t payload = { .one = 0x11, .two = 0x22 };

  fn_status_t result;

  while (true) {
    my_nrf.tx_destination((uint8_t[]) {0x31,0x30,0x30,0x30,0x30});
    result = my_nrf.send_packet(&payload, sizeof(payload));

    if (result)
      printf("Sent:- 0x%x 0x%x\n", payload.one, payload.two);
    else
      printf("Not sent\n");

    payload.two++;

    sleep_ms(1000);
  }
}
