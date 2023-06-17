//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// Third-party includes
#include <NativeEthernet.h>

// Local includes
#include "EthernetNetworkHub.h"
#include "NetworkFactory.h"
#include "NetworkClient.h"
#include "NetworkClientWrapper.h"
#include "NetworkUDP.h"
#include "NetworkUDPWrapper.h"
#include "NetworkServer.h"
#include "NetworkServerWrapper.h"

// NetworkClientWrapper implementation for NativeEthernet EthernetClient.
//
class EthernetClientXWrapper : public NetworkClientWrapper {
  public:
    int connect(IPAddress ip, uint16_t port) { return _ethernetClient.connect(ip, port); };
    int connect(const char *host, uint16_t port) { return _ethernetClient.connect(host, port); };
    size_t write(uint8_t b) { return _ethernetClient.write(b); };
    size_t write(const uint8_t *buf, size_t size) { return _ethernetClient.write(buf, size); };
    int available() { return _ethernetClient.available(); };
    int read() { return _ethernetClient.read(); };
    int read(uint8_t *buf, size_t size) { return _ethernetClient.read(buf, size); };
    int peek() { return _ethernetClient.peek(); };
    void flush() { _ethernetClient.flush(); };
    void stop() { _ethernetClient.stop(); };
    uint8_t connected() { return _ethernetClient.connected(); };
    operator bool() { return _ethernetClient ? true : false; };
    
  private:
    friend class EthernetServerXWrapper;
    
    EthernetClientXWrapper(EthernetClient& ethernetClient) {
      _ethernetClient = ethernetClient;
    };
    
    EthernetClient _ethernetClient;
};

// NetworkServerWrapper implementation for NativeEthernet EthernetServer.
//
class EthernetServerXWrapper : public NetworkServerWrapper {
  public:
    NetworkClient available() {
      EthernetClient ethernetClient = _ethernetServer.available();
      
      EthernetClientXWrapper* clientWrapper = new EthernetClientXWrapper(ethernetClient);
      
      return NetworkFactory::createNetworkClient(clientWrapper);
    };
    
    void begin() { _ethernetServer.begin(); };
    size_t write(uint8_t b) { return _ethernetServer.write(b); };
    size_t write(const uint8_t *buf, size_t size) { return _ethernetServer.write(buf, size); };
    
  private:
    friend class EthernetNetworkHub;
    
    EthernetServerXWrapper(EthernetServer& ethernetServer) {
      _ethernetServer = ethernetServer;
    };
    
    EthernetServer _ethernetServer;
};

// NetworkUDPWrapper implementation for NativeEthernet EthernetUDP.
//
class EthernetUDPXWrapper : public NetworkUDPWrapper {
  public:
    
    uint8_t begin(uint16_t port) { return _ethernetUDP.begin(port); };
    uint8_t beginMulticast(IPAddress ip, uint16_t port) { return _ethernetUDP.beginMulticast(ip, port); };
    void stop() { _ethernetUDP.stop(); };
    int beginPacket(IPAddress ip, uint16_t port) { return _ethernetUDP.beginPacket(ip, port); };
    int beginPacket(const char *host, uint16_t port) { return _ethernetUDP.beginPacket(host, port); };
    int endPacket() { return _ethernetUDP.endPacket(); };
    size_t write(uint8_t b) { return _ethernetUDP.write(b); };
    size_t write(const uint8_t *buffer, size_t size) { return _ethernetUDP.write(buffer, size); };
    int parsePacket() { return _ethernetUDP.parsePacket(); };
    int available() { return _ethernetUDP.available(); };
    int read() { return _ethernetUDP.read(); };
    int read(unsigned char* buffer, size_t len) { return _ethernetUDP.read(buffer, len); };
    int read(char* buffer, size_t len) { return _ethernetUDP.read(buffer, len); };
    int peek() { return _ethernetUDP.peek(); };
    void flush() { _ethernetUDP.flush(); };
    IPAddress remoteIP() { return _ethernetUDP.remoteIP(); };
    uint16_t remotePort() { return _ethernetUDP.remotePort(); };
    
  private:
    friend class EthernetNetworkHub;
    
    EthernetUDPXWrapper(EthernetUDP& ethernetUDP) {
      _ethernetUDP = ethernetUDP;
    };
    
    EthernetUDP _ethernetUDP;
};

bool EthernetNetworkHub::begin(uint8_t *macAddress, Print* printer) {

  bool noError = true;
  
  // start the Ethernet connection
  if (_localIPAddressSet) {
    printer->print("Connecting with ip address ");
    printer->println(_localIPAddress);
    Ethernet.begin(macAddress, _localIPAddress);
  } else {
    printer->println("Connecting with ip address from DHCP");
    int status = Ethernet.begin(macAddress);
    if (status == 0) {
      printer->println("Failed to configure Ethernet using DHCP");
      noError = false;
    }
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    printer->println("Ethernet hardware was not found.");
    noError = false;
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
    printer->println("Ethernet cable is not connected.");
    noError = false;
  }
  
  return noError;
}

void EthernetNetworkHub::setHostIPAddress(IPAddress hostIPAddress) {
  _localIPAddress = hostIPAddress;
  _localIPAddressSet = true;
}
    
NetworkUDP* EthernetNetworkHub::getUdpPort(uint32_t portNum) {
  EthernetUDP udp;
  
  EthernetUDPXWrapper* udpWrapper = new EthernetUDPXWrapper(udp);
  udpWrapper->begin(portNum);
  
  return NetworkFactory::createNetworkUDP(udpWrapper);
}

NetworkServer* EthernetNetworkHub::getTCPServer(uint32_t portNum) {
  EthernetServer tcpServer(portNum);
  
  EthernetServerXWrapper* serverWrapper = new EthernetServerXWrapper(tcpServer);
  serverWrapper->begin();
  
  return NetworkFactory::createNetworkServer(serverWrapper);
}

void EthernetNetworkHub::printStatus(Print* printer) {
  uint8_t mac_address[6];
  Ethernet.MACAddress(mac_address);
  IPAddress localIP = Ethernet.localIP();
  IPAddress subnetMask = Ethernet.subnetMask();
  IPAddress gatewayIP = Ethernet.gatewayIP();
  IPAddress dhcpServerIP = Ethernet.dhcpServerIP();
  IPAddress dnsServerIP = Ethernet.dnsServerIP();
  
  printer->print("Hardware Status: ");
  switch(Ethernet.hardwareStatus()) {
    case EthernetNoHardware:
      printer->println("No Hardware");
      break;
      
    case EthernetW5100:
      printer->println("EthernetW5100");
      break;
      
    case EthernetW5200:
      printer->println("EthernetW5200");
      break;
      
    case EthernetW5500:
      printer->println("EthernetW5500");
      break;
  }
  
  printer->print("Link Status: ");
  switch (Ethernet.linkStatus()) {
    case Unknown:
      printer->println("Unknown");
      break;
      
    case LinkON:
      printer->println("Connected");
      break;

    case LinkOFF:
      printer->println("Not connected");
      break;
  }
  
  printer->print("MAC Address: ");
  for (size_t x = 0; x < sizeof(mac_address); x++) {
    printer->print(mac_address[x], HEX);
    if (x < sizeof(mac_address) - 1) {
      printer->print(":");
    }
  }
  printer->println();
  
  printer->print("IP Address: ");
  printer->println(localIP);
  
  printer->print("Subnet Mask: ");
  printer->println(subnetMask);
  
  printer->print("Gateway IP: ");
  printer->println(gatewayIP);
  
  printer->print("DHCP Server IP: ");
  printer->println(dhcpServerIP);
  
  printer->print("DNS Server IP: ");
  printer->println(dnsServerIP);
}

// Static members and methods

EthernetNetworkHub* EthernetNetworkHub::_ethernetNetworkHub = NULL;

// Returns the instance of EthernetNetworkHub
EthernetNetworkHub EthernetNetworkHub::getInstance() {
  if (_ethernetNetworkHub == NULL) {
    _ethernetNetworkHub = new EthernetNetworkHub();
  }
  return *_ethernetNetworkHub;
};
    
