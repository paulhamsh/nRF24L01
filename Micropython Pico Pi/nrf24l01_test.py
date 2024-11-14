"""Test for nrf24l01 module.  Portable between MicroPython targets."""
import utime
from machine import Pin, SPI
from nrf24l01 import NRF24L01
from micropython import const

_RX_POLL_DELAY = const(15)
_SLAVE_SEND_DELAY = const(10)


# Addresses are in little-endian format. They correspond to big-endian
# 0xf0f0f0f0e1, 0xf0f0f0f0d2
#pipes = (b"\xe1\xf0\xf0\xf0\xf0", b"\xd2\xf0\xf0\xf0\xf0")
#pipes = (b"\x31\x30\x30\x30\x30", b"\xe7\xe7\xe7\xe7\xe7")
#pipes = (b"\x30\x30\x30\x30\x31", b"\x30\x30\x30\x30\x31")
pipes = (b"\x31\x30\x30\x30\x30", b"\x31\x30\x30\x30\x30")

def master():
    csn = Pin(17, mode=Pin.OUT, value=1)
    ce = Pin(22, mode=Pin.OUT, value=0)
    nrf = NRF24L01(SPI(0), csn, ce, channel = 80, payload_size=2)

    nrf.open_tx_pipe(pipes[0])
    #nrf.open_rx_pipe(1, pipes[1])
    
    nrf.print_details()
    
    nrf.start_listening()

    num_needed = 16
    num_successes = 0
    num_failures = 0
    led_state = 0

    print("NRF24L01 master mode, sending %d packets..." % num_needed)

    while num_successes < num_needed and num_failures < num_needed:
        # stop listening and send packet
        nrf.stop_listening()
        print("sending ")
        try:
            nrf.send(b"\x33\x34")
        except OSError:
            print("failed")
            num_failures += 1
            
        num_successes += 1

        """
        # start listening again
        nrf.start_listening()

        # wait for response, with 250ms timeout
        start_time = utime.ticks_ms()
        timeout = False
        while not nrf.any() and not timeout:
            if utime.ticks_diff(utime.ticks_ms(), start_time) > 250:
                timeout = True

        if timeout:
            print("failed, response timed out")
            num_failures += 1
        """
        # delay then loop
        utime.sleep_ms(250)

    print("master finished sending; successes=%d, failures=%d" % (num_successes, num_failures))


def slave():
    csn = Pin(17, mode=Pin.OUT, value=1)
    ce = Pin(22, mode=Pin.OUT, value=0)
    nrf = NRF24L01(SPI(0), csn, ce, channel = 80, payload_size=32)

    nrf.open_tx_pipe(pipes[1])
    nrf.open_rx_pipe(1, pipes[0])
    nrf.start_listening()

    print("NRF24L01 slave mode, waiting for packets... (ctrl-C to stop)")

    while True:
        if nrf.any():
            while nrf.any():
                buf = nrf.recv()
                print("received:", buf)
                utime.sleep_ms(_RX_POLL_DELAY)

            # Give master time to get into receive mode.
            utime.sleep_ms(_SLAVE_SEND_DELAY)
            nrf.stop_listening()
            #nrf.send(b"\x43\x44")

            print("sent response")
            nrf.start_listening()


print("NRF24L01 test module loaded")
print("run nrf24l01test.slave() on slave, then nrf24l01test.master() on master")

master()
