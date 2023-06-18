//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//
 
#ifndef WIFINETWORKHUB_H
#define WIFINETWORKHUB_H

#include "NetworkHub.h"

// A network hub based on a WiFi connection using
// an Adafruit AirLift co-processor or equivalent
// ESP32 board. It requires the Adafruit fork of
// the Arduino WiFiNINA library located here:
//   https://github.com/adafruit/WiFiNINA
//
// It uses the SPI interface with a couple extra
// pins for reset and busy. There are a couple of
// extra methods to set the pins, wifi network,
// and wifi password.
//
class WiFiNetworkHub : public NetworkHub {
  public:
    // The pins used by the AirLift/ESP32 board.
    void setPins(uint8_t spiMOSIPin, uint8_t spiMISOPin, uint8_t spiSCKPin,
      uint8_t spiCSPin, uint8_t resetPin, uint8_t busyPin);
    
    // Start the network hub, connecting to the WiFi network (ssid)
    // using the given password.
    bool begin(const char* ssid, const char* password, Print* printer);
    
    // Stop the network hub
    void stop(void);
    
    // Implementations of NetworkHub interface
    IPAddress getLocalIPAddress();
    NetworkClient getClient();
    NetworkServer* getServer(uint32_t portNum);
    NetworkUDP* getUDP();
    void printStatus(Print* printer);
    
    // Returns the singleton instance of WiFiNetworkHub
    static WiFiNetworkHub getInstance();
    
  protected:
    WiFiNetworkHub() { /* Nothing to see here, move along. */ };
    
    static WiFiNetworkHub* _wifiNetworkHub;
};

#endif // WIFINETWORKHUB_H
