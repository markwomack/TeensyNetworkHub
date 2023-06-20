//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//
 
#ifndef NATIVEETHERNETNETWORKHUB_H
#define NATIVEETHERNETNETWORKHUB_H

#include <Arduino.h>

#include "NetworkHub.h"

// A network hub based on an ethernet connection
// implemented by NativeEthernet library.
//   https://github.com/vjmuzik/NativeEthernet
//
class NativeEthernetNetworkHub : public NetworkHub {
  public:
    
    bool begin(uint8_t *macAddress, Print* printer);
    
    // NetworkHub methods
    IPAddress getLocalIPAddress();
    NetworkClient getClient();
    NetworkServer* getServer(uint32_t portNum);
    NetworkUDP* getUDP();
    void printStatus(Print* printer);
    
    // Returns the singleton instance of EthernetNetworkHub
    static NativeEthernetNetworkHub getInstance();
    
  protected:
    NativeEthernetNetworkHub() { /* Nothing to see here, move along. */ };
    
    static NativeEthernetNetworkHub* _nativeEthernetNetworkHub;
};

#endif // NATIVEETHERNETNETWORKHUB_H
