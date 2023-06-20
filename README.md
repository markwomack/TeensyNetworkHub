# TeensyNetworkHub
TeensyNetworkHub implements a basic network hub interface for Teensy microcontrollers which provides access
to TCP and UDP. It is implemented with a common set of classes that can be used with either WiFi or native
ethernet. The purpose of this library is to make it easy to switch code from using WiFi as the underlying
network to using native ethernet, and vica-versa. Switching your sketch code between WiFi and native ethernet
versions only requires a few lines of changes, with all of the primary usage remaining the same.

## WiFiNINA Library
This version of the network hub implemented to work with the [Adafruit AirLift board](https://www.adafruit.com/product/4201)
or comparable ESP32 board. The code requires that the
[Adafruit fork of the WiFiNINA library](https://github.com/adafruit/WiFiNINA/) be available in your Arduino
library path. The latest version of the WiFiNINA library (from Arduino), is not compatible due to the support
for different SPI pins added in the Adafruit fork.

Use the [WiFiNINANetworkHub](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/WiFiNINANetworkHub.h) class
in your code to use this implementation.

## QNEthernet Library
This version of the network hub is implemented to work with the
[QNEthernet library](https://github.com/ssilverman/QNEthernet).

Use the [QNEthernetNetworkHub](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/QNEthernetNetworkHub.h) class
in your code to use this implementation.

## Native Ethernet Library
This version of the network hub is implemented to work with the
[NativeEthernet library](https://github.com/vjmuzik/NativeEthernet) that ships as part of the
[Teensyduino installation](https://www.pjrc.com/teensy/td_download.html).

Use the [NativeEthernetNetworkHub](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NativeEthernetNetworkHub.h) class
in your code to use this implementation.

## NetworkHub
The [NetworkHub interface](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NetworkHub.h) defines
the common interface provided for all subclass specific versions
([WiFi](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/WiFiNINANetworkHub.h),
[QNEthernet](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/QNEthernetNetworkHub.h),
[NativeEthernet](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NativeEthernetNetworkHub.h)). It provides
methods for creating the primary objects to interact with the underlying network.

### [NetworkClient](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NetworkClient.h)
Defines the interface for client interface to the network. The **getClient** method of NetworkHub should be
called to create a new instance. Instances are also returned by NetworkServer.available().

### [NetworkServer](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NetworkServer.h)
Defines the interface for a server interface to the network. The **getServer** method of NetworkHub should be
called to create a new instance.

### [NetworkUDP](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/NetworkUDP.h)
Defines the interface for a UDP interface to the network. The **getUDP** method of NetworkHub should be called
to create a new instance.

## Examples
The included [examples](https://github.com/markwomack/TeensyNetworkHub/tree/main/examples) come from the original
NativeEthernet examples. They demonstrate basic usage for NetworkClient, NetworkServer, and NetworkUDP.

## Extending
If you have a favorite network library for connecting to the internet, it is easy to extend TeensyNetworkHub to
support it. You will need to implement wrappers to integrate the library with TeensyNetworkHub, and implement
a version of NetworkHub to iniitalize the connection and provide the clients, servers, and UDPs. Please see
[WiFiNetworkHub](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/WiFiNetworkHub.h),
[QNEthernetNetworkHub](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/QNEthernetNetworkHub.h), or
[EthernetNetworkHub](https://github.com/markwomack/TeensyNetworkHub/blob/main/src/EthernetNetworkHub.h) for examples.

Just as an example, I was able to add the implementation for the QNEthernet library in under and hour, with another
hour to work out some kinks.

## Suggestions, Bugs, Concerns
If you have any suggestions or bugs, please [open an issue](https://github.com/markwomack/TeensyNetworkHub/issues).
