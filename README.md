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


<p align="center">
  <img src="https://github.com/paulhamsh/nRF24L01/blob/main/nRF24L01Pinout2.jpg" width="600" title="nRF24L01 Addresses">
</p>

### Addresses for nRF24L01

This is my understanding of the addressing scheme.   
Forget a notion that each nRF24L01 has 'an address'. This is not helpful. Each nRF24L01 can 'listen' to up to six addresses, and therefore process messages sent to these addresses.  

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


