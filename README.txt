== WiFly Arduino Library ==

This is a library for working with the WiFly GSX and EZX devices from
Roving Networks with Arduino 1.0 and above.

The goal with this library is to make it--as much as possible--a "drop
in" replacement for the official Arduino Ethernet library
<http://www.arduino.cc/en/Reference/Ethernet>. Once a wireless network
is joined the library should respond in the same way as the Ethernet
library. This means you should be able to take existing Ethernet
examples and make them work wirelessly without too many changes.

The library also provides a high-level interface for the "SC16IS750
I2C/SPI-to-UART IC" used in the WiFly shield but also available on a
separate breakout board:

   <http://www.sparkfun.com/products/9981>

= Usage =

This version of the library provides a simple wrapper to the WiFly
device. As such, it's important to read and understand the WiFly
command set.