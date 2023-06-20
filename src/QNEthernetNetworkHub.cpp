//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// Third-party includes
#include <QNEthernet.h>  // https://github.com/ssilverman/QNEthernet

// Local includes
#include "QNEthernetNetworkHub.h"
#include "NetworkFactory.h"
#include "NetworkClient.h"
#include "NetworkClientWrapper.h"
#include "NetworkUDP.h"
#include "NetworkUDPWrapper.h"
#include "NetworkServer.h"
#include "NetworkServerWrapper.h"

using namespace qindesign::network;

// NetworkClientWrapper implementation for QNEthernet EthernetClient.
//
class QNEthernetClientWrapper : public NetworkClientWrapper {
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
    IPAddress remoteIP() { return _ethernetClient.remoteIP(); };
    uint16_t remotePort() { return _ethernetClient.remotePort(); };
    
    NetworkClientWrapper* clone() {
      return new QNEthernetClientWrapper(_ethernetClient);
    }
    
  private:
    friend class QNEthernetNetworkHub;
    friend class QNEthernetServerWrapper;
    
    QNEthernetClientWrapper() {};
    QNEthernetClientWrapper(EthernetClient& ethernetClient) {
      _ethernetClient = ethernetClient;
    };
    
    EthernetClient _ethernetClient;
};

// NetworkServerWrapper implementation for QNEthernet EthernetServer.
//
class QNEthernetServerWrapper : public NetworkServerWrapper {
  public:
    NetworkClient available() {
      EthernetClient ethernetClient = _ethernetServer->available();
      
      QNEthernetClientWrapper* clientWrapper = new QNEthernetClientWrapper(ethernetClient);
      
      return NetworkFactory::createNetworkClient(clientWrapper);
    };
    
    void begin() { _ethernetServer->begin(); };
    size_t write(uint8_t b) { return _ethernetServer->write(b); };
    size_t write(const uint8_t *buf, size_t size) { return _ethernetServer->write(buf, size); };
    
    ~QNEthernetServerWrapper() {
      delete _ethernetServer;
    };
    
  private:
    friend class QNEthernetNetworkHub;
    
    QNEthernetServerWrapper(EthernetServer* ethernetServer) {
      _ethernetServer = ethernetServer;
    };
    
    EthernetServer* _ethernetServer;
};

// NetworkUDPWrapper implementation for QNEthernet EthernetUDP.
//
class QNEthernetUDPWrapper : public NetworkUDPWrapper {
  public:
    
    uint8_t begin(uint16_t port) { return _ethernetUDP->begin(port); };
    uint8_t beginMulticast(IPAddress ip, uint16_t port) { return _ethernetUDP->beginMulticast(ip, port); };
    void stop() { _ethernetUDP->stop(); };
    int beginPacket(IPAddress ip, uint16_t port) { return _ethernetUDP->beginPacket(ip, port); };
    int beginPacket(const char *host, uint16_t port) { return _ethernetUDP->beginPacket(host, port); };
    int endPacket() { return _ethernetUDP->endPacket(); };
    size_t write(uint8_t b) { return _ethernetUDP->write(b); };
    size_t write(const uint8_t *buffer, size_t size) { return _ethernetUDP->write(buffer, size); };
    int parsePacket() { return _ethernetUDP->parsePacket(); };
    int available() { return _ethernetUDP->available(); };
    int read() { return _ethernetUDP->read(); };
    int read(unsigned char* buffer, size_t len) { return _ethernetUDP->read(buffer, len); };
    int read(char* buffer, size_t len) { return _ethernetUDP->read(buffer, len); };
    int peek() { return _ethernetUDP->peek(); };
    void flush() { _ethernetUDP->flush(); };
    IPAddress remoteIP() { return _ethernetUDP->remoteIP(); };
    uint16_t remotePort() { return _ethernetUDP->remotePort(); };
    
    ~QNEthernetUDPWrapper() {
      delete _ethernetUDP;
    };
    
  private:
    friend class QNEthernetNetworkHub;
    
    QNEthernetUDPWrapper(EthernetUDP* ethernetUDP) {
      _ethernetUDP = ethernetUDP;
    };
    
    EthernetUDP* _ethernetUDP;
};

bool QNEthernetNetworkHub::begin(Print* printer) {

  bool hadError = false;
  
  // start the Ethernet connection
  if (hasConfiguredLocalIPAddress()) {
    printer->print("Connecting with ip address ");
    printer->println(getConfiguredLocalIPAddress());
    Ethernet.begin(getConfiguredLocalIPAddress(), getConfiguredSubnetMask(),
      getConfiguredGatewayIPAddress(),getConfiguredDNSIPAddress());
  } else {
    printer->println("Connecting with ip address from DHCP");
    Ethernet.begin();
    IPAddress ip = Ethernet.waitForLocalIP(30000);
    if (!Ethernet.isDHCPActive() || ip == INADDR_NONE) {
      printer->println("Failed to configure Ethernet using DHCP");
      hadError = true;
    }
  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    printer->println("Ethernet hardware was not found.");
    hadError = true;
  }
  
  if (Ethernet.linkStatus() == LinkOFF) {
    printer->println("Ethernet cable is not connected.");
    hadError = true;
  }
  
  return !hadError;
}

IPAddress QNEthernetNetworkHub::getLocalIPAddress() {
  return Ethernet.localIP();
}

NetworkClient QNEthernetNetworkHub::getClient() {
  QNEthernetClientWrapper* clientWrapper = new QNEthernetClientWrapper();
  
  return NetworkFactory::createNetworkClient(clientWrapper);
}

NetworkServer* QNEthernetNetworkHub::getServer(uint32_t portNum) {
  EthernetServer* server = new EthernetServer(portNum);
  
  QNEthernetServerWrapper* serverWrapper = new QNEthernetServerWrapper(server);
  
  return NetworkFactory::createNetworkServer(serverWrapper);
}
    
NetworkUDP* QNEthernetNetworkHub::getUDP() {
  EthernetUDP* udp = new EthernetUDP();
  
  QNEthernetUDPWrapper* udpWrapper = new QNEthernetUDPWrapper(udp);
  
  return NetworkFactory::createNetworkUDP(udpWrapper);
}


void QNEthernetNetworkHub::printStatus(Print* printer) {
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
      
    default:
      printer->println("Unknown hardware");
      break;
  }
  
  printer->print("Link Status: ");
  printer->println(Ethernet.linkState() ? "Connected" : "Not Connected");
  
  if (Ethernet.linkState()) {
    printer->print("Link Speed: ");
    printer->println(Ethernet.linkSpeed());
    
    printer->print("Duplex mode: ");
    printer->println(Ethernet.linkIsFullDuplex() ? "Full" : "Half");
    
    printer->print("Crossover: ");
    printer->println(Ethernet.linkIsCrossover() ? "TRUE" : "FALSE");
  }
  
  uint8_t macAddress[6];
  Ethernet.macAddress(macAddress);
  printer->print("MAC Address: ");
  for (size_t x = 0; x < sizeof(macAddress); x++) {
    printer->print(macAddress[x], HEX);
    if (x < sizeof(macAddress) - 1) {
      printer->print(":");
    }
  }
  printer->println();
  
  printer->print("IP Address: ");
  printer->println(Ethernet.localIP());
  
  printer->print("Subnet Mask: ");
  printer->println(Ethernet.subnetMask());
  
  printer->print("Gateway IP: ");
  printer->println(Ethernet.gatewayIP());
  
  printer->print("DNS Server IP: ");
  printer->println(Ethernet.dnsServerIP());
}

// Static members and methods

QNEthernetNetworkHub* QNEthernetNetworkHub::_qnEthernetNetworkHub = NULL;

// Returns the instance of EthernetNetworkHub
QNEthernetNetworkHub QNEthernetNetworkHub::getInstance() {
  if (_qnEthernetNetworkHub == NULL) {
    _qnEthernetNetworkHub = new QNEthernetNetworkHub();
  }
  return *_qnEthernetNetworkHub;
};
    
