# nRF24L01
Examples for ESP32 and Pico Pi to use nRF24L01
Set up for the keyestrudio ESP32 board, an M5Stack Core 2 and two Pico Pis.   

Useful sources:
```
Datasheet:         https://www.sparkfun.com/datasheets/Components/nRF24L01_prelim_prod_spec_1_2.pdf
Nordic:            https://www.nordicsemi.com/Products/nRF24-series
ESP32 library:     https://github.com/nRF24/RF24
Documentation:     https://nrf24.github.io/RF24/
Pico Pi library:   https://github.com/AndyRids/pico-nrf24
                   (This is just one library but I like it - it is clear and logical)   
```                   

The ESP examples here were heavily amended from a variety of sources.     
The Pico Pi eaxmples are derived from the ones here: https://github.com/AndyRids/pico-nrf24/tree/main/examples     

### nRF24L01 pinout

<p align="center">
  <img src="https://github.com/paulhamsh/nRF24L01/blob/main/nrf24L01_pinout.jpg" width="600" title="nRF24L01 pinout">
</p>

### nRF24L01 expander board pinout

<p align="center">
  <img src="https://github.com/paulhamsh/nRF24L01/blob/main/nrf24L01_expander.jpg" width="600" title="nRF24L01 expander">
</p>

### Build for Pico Pi (using wsl)

```
git clone https://github.com/AndyRids/pico-nrf24.git
cd pico-nrf24
export PICO_SDK_PATH=~/pico_base/pico-sdk

cp ~/pico_base/pico-sdk/external/pico_sdk_import.cmake    .
mkdir build
cd build
cmake -DFAMILY=rp2040 ..
make
```

Add the files under ```examples/my_transmitter/```:
```
my_transmitter.c
CMakeLists.txt
```

Edit CMakeLists.txt in ```/pico-nrf24/examples```:

```
add_subdirectory(primary_receiver)
add_subdirectory(primary_transmitter)
add_subdirectory(my_transmitter)
```



### Addresses for nRF24L01

This is my understanding of the addressing scheme.    
Forget a notion that each nRF24L01 has 'an address'. This is not helpful.   
Each nRF24L01 can 'listen' to up to six addresses, and therefore process messages sent to these addresses.   
The transmitting device sends a packet with the chosen address.   
The receiving device will accept the packet if it is accepting that address.   
If auto-acknowledge is active, the receiving device will then transmit an acknowledge using the same address - so the transmitting device must now be listening for that address to receive the acknowledgement.   

<p align="center">
  <img src="https://github.com/paulhamsh/nRF24L01/blob/main/nRF24L01Addresses.jpg" width="800" title="nRF24L01 Addresses">
</p>







### Pico Pi SPI mappings

To check the SPI pins selected are valid, they must match the appropriate SPI bank and also be the right map for that pin type.   
SPI bank can be found from (value & 0x80 >> 3)   
Pin type can be checked with (value & 0x03) as per the table below.   


```
data & 00 00 00 11
00 RX
01 CSn
10 CLK
11 TX

data & 00 00 10 00
00 00   SPI 0
10 00   SPI 1


          765 4 3 2 10                765 4 3 2 10  
SPI 0
1         000 0 0 0 00          17    000 1 0 0 00
2         000 0 0 0 01          18    000 1 0 0 01
3         000 0 0 0 10          19    000 1 0 0 10
4         000 0 0 0 11          20    000 1 0 0 11

5         000 0 0 1 00          21    000 1 0 1 00
6         000 0 0 1 01          22    000 1 0 1 01
7         000 0 0 1 10          23    000 1 0 1 10
8         000 0 0 1 11          24    000 1 0 1 11

SPI 1
9         000 0 1 0 00          25    000 1 1 0 00
10        000 0 1 0 01          26    000 1 1 0 01
11        000 0 1 0 10          27    000 1 1 0 10
12        000 0 1 0 11          28    000 1 1 0 11

13        000 0 1 1 00          29    000 1 1 1 00
14        000 0 1 1 01          30    000 1 1 1 01
15        000 0 1 1 10
16        000 0 1 1 11
```


### Potential simplifications to nrf24 library on Pico C

```
static fn_status_t pin_manager_validate(uint8_t copi, uint8_t cipo, uint8_t sck) {

  // Can test the pins are valid by masking and comparing
  fn_status_t status;
  copi &= 3;
  cipo &= 3;
  sck  &= 3;

  if ((copi == 3) && (cipo == 0) && (sck == 2))
    status =  PIN_MNGR_OK;
  else
    status =  ERROR;

  return status;
}
```

Or, more cryptically, in a single line:

```
return (   ((copi & 3) == 3) && 
           ((cipo & 3) == 0) && 
           ((sck  & 3) == 2)      ) ? PIN_MGR_OK : ERROR;
```

And we can shorten the SPI bank check in ```nrf_driver_configure```

```
   uint8_t spi_sck, spi_copi, spi_cipo, count;

   spi_sck  = (pins->sck  & 8) >> 3;
   spi_copi = (pins->copi & 8) >> 3;
   spi_cipo = (pins->cipo & 8) >> 3;

   count = spi_sck + spi_copi + spi_cipo;

   status = ((count == 3) || (count == 0)) ? PIN_MNGR_OK : ERROR;

   // other lines

     spi->instance = (count == 0) ? spi0 : spi1;

   // other lines
```

Or...

```
   uint8_t spi_bank;

   status = PIN_MNGR_OK;
   spi_bank = (pins->sck  & 8) >> 3;                                // has to be 1 or 0
   if ((pins->copi & 8) >> 3 != spi_bank) status = ERROR;
   if ((pins->cipo & 8) >> 3 != spi_bank) status = ERROR;

   // other lines

     spi->instance = (spi_bank == 0) ? spi0 : spi1;

   // other lines
```

### Adding new function in Pico Pi library

Add code to nrf24_driver.c    

```
/**
 * Read the size of an available packet in the RX FIFO
 *
 * @param size size of buffer
 *
 * @return SPI_MNGR_OK (2), ERROR (0)
 */

fn_status_t nrf_driver_read_packet_size(uint8_t *size) {

  fn_status_t status = SPI_MNGR_OK;

  *size = 0;

  spi_manager_t *spi = &(nrf_driver.user_spi);
  spi_manager_init_spi(spi->instance, spi->baudrate);

  /**
   * if dynamic payloads are enabled, read the payload width
   * via the R_RX_PL_WID command.
   */

  if (nrf_driver.user_config.dyn_payloads)
  {
    uint8_t tx_buffer[2] = { R_RX_PL_WID, NOP };
    uint8_t rx_buffer[2];

    csn_put_low(nrf_driver.user_pins.csn); // drive CSN pin LOW
    status = spi_manager_transfer(spi->instance, tx_buffer, rx_buffer, TWO_BYTES);
    csn_put_high(nrf_driver.user_pins.csn); // drive CSN pin HIGH

    *size = rx_buffer[1];
  }

  spi_manager_deinit_spi(spi->instance);
  return status;
```

Add to ```nrf24_driver.h``` in ```nrf_client_t``` as a new function    

```
// provides access to nrf_driver public functions
typedef struct nrf_client_s
{
  // ..........

  // size of dynamic read buffer
  fn_status_t (*read_packet_size)(uint8_t *size);
} nrf_client_t;
```

Add to  ```nrf24_driver.c``` in ```nrf_driver_create_client``` so it can be called.   

```
fn_status_t nrf_driver_create_client(nrf_client_t *client) {
   // ....
  client->read_packet_size = nrf_driver_read_packet_size;

  return NRF_MNGR_OK;
}
```
