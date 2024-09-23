# nRF24L01
Examples for ESP32 and Pico Pi to use nRF24L01

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


