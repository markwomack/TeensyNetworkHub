# TeensyNetworkHub
A basic network hub interface for Teensy microcontrollers which provides TCP and UDP ports. It is implemented
for WiFi and native ethernet. Switching code between WiFi and native ethernet versions only requires a few
lines of changes, with all of the usage remaining the same.

The WiFi code is implemented to work with the [Adafruit AirLift board](https://www.adafruit.com/product/4201)
or comparable ESP32 board. The code requires that the
[Adafruit fork of the WiFiNINA library](https://github.com/adafruit/WiFiNINA/) be available in your Arduino
library path. The latest version of the WiFiNINA library (from Arduino), is not compatible.

The native ethernet code is implemented to work with the
[NativeEthernet library](https://github.com/vjmuzik/NativeEthernet) that ships as part of the
[Teensyduino installation](https://www.pjrc.com/teensy/td_download.html).

Please see the [basic examples](https://github.com/markwomack/TeensyNetworkHub/tree/main/examples) for usage
documentation.

If you have any suggestions or bugs, please [open an issue](https://github.com/markwomack/TeensyNetworkHub/issues).
