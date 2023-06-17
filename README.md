# TeensyNetworkHub
TeensyNetworkHub implements a basic network hub interface for Teensy microcontrollers which provides access
to TCP and UDP. It is implemented with a common set of classes that can be used with either WiFi or native
ethernet. The purpose of this library is to make it easy to switch code from using WiFi as the underlying
network to using native ethernet, and vica-versa. Switching your sketch code between WiFi and native ethernet
versions only requires a few lines of changes, with all of the primary usage remaining the same.

## WiFi
The WiFi code is implemented to work with the [Adafruit AirLift board](https://www.adafruit.com/product/4201)
or comparable ESP32 board. The code requires that the
[Adafruit fork of the WiFiNINA library](https://github.com/adafruit/WiFiNINA/) be available in your Arduino
library path. The latest version of the WiFiNINA library (from Arduino), is not compatible due to the support
for different SPI pins added in the Adafruit fork.

## Native Ethernet
The native ethernet code is implemented to work with the
[NativeEthernet library](https://github.com/vjmuzik/NativeEthernet) that ships as part of the
[Teensyduino installation](https://www.pjrc.com/teensy/td_download.html).

## NetworkHub
The [NetworkHub interface](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NetworkHub.h) defines
the common interface provided for all subclass specific versions
([WiFi](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/WiFiNetworkHub.h), 
[Native Ethernet](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/EthernetNetworkHub.h)). It provides
methods for creating the primary objects to interact with the underlying network.

### NetworkClient
Defines the interface for client interface to the network. The **getClient** method of NetworkHub should be
called to create a new instance. Instances are also returned by NetworkServer.available().

### NetworkServer
Defines the interface for a server interface to the network. The **getServer** method of NetworkHub should be
called to create a new instance.

### NetworkUDP
Defines the interface for a UDP interface to the network. The **getUDP** method of NetworkHub should be called
to create a new instance.

## Examples
Please see the [basic examples](https://github.com/markwomack/TeensyNetworkHub/tree/main/examples) for usage
documentation.

## Extending

If you have any suggestions or bugs, please [open an issue](https://github.com/markwomack/TeensyNetworkHub/issues).
