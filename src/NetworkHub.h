//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKHUB_H
#define NETWORKHUB_H

#include <Arduino.h>

#include "NetworkClient.h"
#include "NetworkServer.h"
#include "NetworkUDP.h"

// Generic interface for the network hub. Implementations
// must implement the methods. The objects used to interact
// with the network are created by calls to the hub, and
// should not be created independently.
//
class NetworkHub {
  public:
    // Set a fixed IP address for this device. This is an
    // optional call, otherwise an IP address will be
    // assigned from the DHCP server.
    void setLocalIPAddress(IPAddress localIPAddress) {
      _localIPAddress = localIPAddress;
      _hasLocalIPAddress = true;
    };
    
    void setDNSIPAddress(IPAddress dnsIPAddress) {
      _dnsIPAddress = dnsIPAddress;
      _hasDNSIPAddress = true;
    };
    
    void setGatewayIPAddress(IPAddress gatewayIPAddress) {
      _gatewayIPAddress = gatewayIPAddress;
      _hasGatewayIPAddress = true;
    };
    
    void setSubnetMask(IPAddress subnetMask) {
      _subnetMask = subnetMask;
      _hasSubnetMask = true;
    };
    
    // These methods must be implemented by subclasses
    
    virtual IPAddress getLocalIPAddress() = 0;
    
    // Create a client to access the network.
    // Returns a NetworkClient for use.
    virtual NetworkClient getClient() = 0;
    
    // Create a TCP server for the given port number.
    // Returns a pointer to a NetworkServer for use.
    virtual NetworkServer* getServer(uint32_t portNum) = 0;

    // Create a UDP port.
    // Returns a pointer to a NetworkUDP for use.
    virtual NetworkUDP* getUDP() = 0;
    
    // Print the status of the hub to the given
    // Print object (ie Serial).
    virtual void printStatus(Print* printer) = 0;
    
  protected:
    NetworkHub() { /* Nothing to see here, move along. */ }
    
    bool _hasLocalIPAddress = false;
    IPAddress _localIPAddress;
    bool _hasDNSIPAddress = false;
    IPAddress _dnsIPAddress;
    bool _hasGatewayIPAddress = false;
    IPAddress _gatewayIPAddress;
    bool _hasSubnetMask = false;
    IPAddress _subnetMask;
    
    // Common methods for all subclasses
    
    bool hasConfiguredLocalIPAddress() {
      return _hasLocalIPAddress;
    };
    
    IPAddress getConfiguredLocalIPAddress() {
      return _localIPAddress;
    };
    
    bool hasConfiguredDNSIPAddress() {
      return _hasDNSIPAddress;
    };
    
    IPAddress getConfiguredDNSIPAddress() {
      // If no DNS ip address has been set, but a local ip address has,
      // then assume the DNS is the local ip with '1' for the last element.
      if (!_hasDNSIPAddress && _hasLocalIPAddress) {
        _dnsIPAddress = _localIPAddress;
        _dnsIPAddress[3] = 1;
      }
      return _dnsIPAddress;
    };
    
    bool hasConfiguredGatewayIPAddress() {
      return _hasGatewayIPAddress;
    };
    
    IPAddress getConfiguredGatewayIPAddress() {
      // If no gateway ip address has been set, but a local ip address has,
      // then assume the gateway is the local ip with '1' for the last element.
      if (!_hasGatewayIPAddress && _hasLocalIPAddress) {
        _gatewayIPAddress = _localIPAddress;
        _gatewayIPAddress[3] = 1;
      }
      return _gatewayIPAddress;
    };
    
    bool hasConfiguredSubnetMask() {
      return _hasSubnetMask;
    };
    
    IPAddress getConfiguredSubnetMask() {
      // If no subnet has been set, then assume the
      // subnet mask should be 255.255.255.0
      if (!_hasSubnetMask) {
        _subnetMask = IPAddress(255, 255, 255, 0);
      }
      return _subnetMask;
    };
};

#endif // NETWORKHUB_H
