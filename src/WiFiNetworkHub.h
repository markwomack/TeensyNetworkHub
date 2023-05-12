//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//
 
#ifndef WIFINETWORKHUB_H
#define WIFINETWORKHUB_H

//
// A network hub based on a WiFi connection, that can
// return TCP and UDP ports for use. The code is expecting
// to work with an Adafruit AirLift co-processor or
// equivalent ESP32 board.
//

#include <Arduino.h>
#include <Udp.h>
#include <WiFiServer.h>

class WiFiNetworkHub {
  public:
    WiFiNetworkHub() { /* Nothing to see here, move along. */ }
  
    // The pins used by the AirLift/ESP32 board.
    void setPins(uint8_t spiMOSIPin, uint8_t spiMISOPin, uint8_t spiSCKPin,
      uint8_t spiCSPin, uint8_t resetPin, uint8_t busyPin);
      
    // Set a fixed IP address for the host (optional).
    void setHostIPAddress(IPAddress hostIPAddress);
    
    // Start the network hub, connecting to the WiFi network (ssid)
    // using the given password.
    bool start(const char* ssid, const char* password, Print* printer);
    
    // Stop the network hub, disconnect from the WiFi network.
    void stop(void);

    // Open a UDP port on the given port number.
    // Returns a UDP pointer for use.
    UDP* getUdpPort(uint32_t portNum);
    
    // Open a TCP port on the given port number.
    // Returns a WiFiServer pointer for use.
    WiFiServer* getTCPServer(uint32_t portNum);
    
    // Print the status of the hub to the given
    // Print object (ie Serial).
    void printWiFiStatus(Print* printer);
};

#endif // WIFINETWORKHUB_H
