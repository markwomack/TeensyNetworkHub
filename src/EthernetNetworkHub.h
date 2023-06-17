//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//
 
#ifndef ETHERNETNETWORKHUB_H
#define ETHERNETNETWORKHUB_H

#include <Arduino.h>

#include "NetworkHub.h"

// A network hub based on an ethernet connection
// implemented by NativeEthernet library.
//   https://github.com/vjmuzik/NativeEthernet
//
class EthernetNetworkHub : public NetworkHub {
  public:
    
    bool begin(uint8_t *macAddress, Print* printer);
    
    // NetworkHub methods
    void setHostIPAddress(IPAddress hostIPAddress);
    NetworkUDP* getUdpPort(uint32_t portNum);
    NetworkServer* getTCPServer(uint32_t portNum);
    void printStatus(Print* printer);
    
    // Returns the singleton instance of EthernetNetworkHub
    static EthernetNetworkHub getInstance();
    
  protected:
    EthernetNetworkHub() { /* Nothing to see here, move along. */ };
    
    static EthernetNetworkHub* _ethernetNetworkHub;
    
    IPAddress _localIPAddress;
    bool _localIPAddressSet = false;
};

#endif // ETHERNETNETWORKHUB_H
