//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//
 
#ifndef QNETHERNETNETWORKHUB_H
#define QNETHERNETNETWORKHUB_H

#include <Arduino.h>

#include "NetworkHub.h"

// A network hub based on an ethernet connection
// implemented by QNEthernet library.
//   https://github.com/ssilverman/QNEthernet
//
class QNEthernetNetworkHub : public NetworkHub {
  public:
    
    bool begin(Print* printer);
    
    // NetworkHub methods
    IPAddress getLocalIPAddress();
    NetworkClient getClient();
    NetworkServer* getServer(uint32_t portNum);
    NetworkUDP* getUDP();
    void printStatus(Print* printer);
    
    // Returns the singleton instance of EthernetNetworkHub
    static QNEthernetNetworkHub getInstance();
    
  protected:
    QNEthernetNetworkHub() { /* Nothing to see here, move along. */ };
    
    static QNEthernetNetworkHub* _qnEthernetNetworkHub;
};

#endif // QNETHERNETNETWORKHUB_H
