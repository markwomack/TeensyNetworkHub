//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// Third-party includes
#include <SPI.h>
#include <WiFiNINA.h> // https://github.com/adafruit/WiFiNINA
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

// Local includes
#include "WiFiNetworkHub.h"
#include "NetworkFactory.h"
#include "NetworkClient.h"
#include "NetworkClientWrapper.h"
#include "NetworkUDP.h"
#include "NetworkUDPWrapper.h"
#include "NetworkServer.h"
#include "NetworkServerWrapper.h"

// NetworkClientWrapper implementation for WiFiNINA WiFiClient.
//
class WiFiClientWrapper : public NetworkClientWrapper {
  public:
    
    int connect(IPAddress ip, uint16_t port) { return _wifiClient.connect(ip, port); };
    int connect(const char *host, uint16_t port) { return _wifiClient.connect(host, port); };
    int connectSSL(IPAddress ip, uint16_t port) { return _wifiClient.connectSSL(ip, port); };
    int connectSSL(const char *host, uint16_t port) { return _wifiClient.connectSSL(host, port); };
    size_t write(uint8_t b) { return _wifiClient.write(b); };
    size_t write(const uint8_t *buf, size_t size) { return _wifiClient.write(buf, size); };
    int available() { return _wifiClient.available(); };
    int read() { return _wifiClient.read(); };
    int read(uint8_t *buf, size_t size) { return _wifiClient.read(buf, size); };
    int peek() { return _wifiClient.peek(); };
    void flush() { _wifiClient.flush(); };
    void stop() { _wifiClient.stop(); };
    uint8_t connected() { return _wifiClient.connected(); };
    operator bool() { return _wifiClient ? true : false; };
    IPAddress remoteIP() { return _wifiClient.remoteIP(); };
    uint16_t remotePort() { return _wifiClient.remotePort(); };
    
  private:
    friend class WiFiNetworkHub;
    friend class WiFiServerWrapper;
    
    WiFiClientWrapper(WiFiClient& wifiClient) {
      _wifiClient = wifiClient;
    };
    
    WiFiClient _wifiClient;
};

// NetworkServerWrapper implementation for WiFiNINA WiFiServer.
//
class WiFiServerWrapper : public NetworkServerWrapper {
  public:
    NetworkClient available() {
      WiFiClient wifiClient = _wifiServer->available();
      
      WiFiClientWrapper* clientWrapper = new WiFiClientWrapper(wifiClient);
      
      return NetworkFactory::createNetworkClient(clientWrapper);
    };
    
    void begin() { _wifiServer->begin(); };
    size_t write(uint8_t b) { return _wifiServer->write(b); };
    size_t write(const uint8_t *buf, size_t size) { return _wifiServer->write(buf, size); };
    
    ~WiFiServerWrapper() {
      delete _wifiServer;
    };
    
  private:
    friend class WiFiNetworkHub;
    
    WiFiServerWrapper(WiFiServer* wifiServer) {
      _wifiServer = wifiServer;
    };
    
    WiFiServer* _wifiServer;
};

// NetworkUDPWrapper implementation for WiFiNINA WiFiUDP.
//
class WiFiUDPWrapper : public NetworkUDPWrapper {
  public:
    
    uint8_t begin(uint16_t port) { return _wifiUDP->begin(port); };
    uint8_t beginMulticast(IPAddress ip, uint16_t port) { return _wifiUDP->beginMulticast(ip, port); };
    void stop() { _wifiUDP->stop(); };
    int beginPacket(IPAddress ip, uint16_t port) { return _wifiUDP->beginPacket(ip, port); };
    int beginPacket(const char *host, uint16_t port) { return _wifiUDP->beginPacket(host, port); };
    int endPacket() { return _wifiUDP->endPacket(); };
    size_t write(uint8_t b) { return _wifiUDP->write(b); };
    size_t write(const uint8_t *buffer, size_t size) { return _wifiUDP->write(buffer, size); };
    int parsePacket() { return _wifiUDP->parsePacket(); };
    int available() { return _wifiUDP->available(); };
    int read() { return _wifiUDP->read(); };
    int read(unsigned char* buffer, size_t len) { return _wifiUDP->read(buffer, len); };
    int read(char* buffer, size_t len) { return _wifiUDP->read(buffer, len); };
    int peek() { return _wifiUDP->peek(); };
    void flush() { _wifiUDP->flush(); };
    IPAddress remoteIP() { return _wifiUDP->remoteIP(); };
    uint16_t remotePort() { return _wifiUDP->remotePort(); };
    
    ~WiFiUDPWrapper() {
      delete _wifiUDP;
    };
    
  private:
    friend class WiFiNetworkHub;
    
    WiFiUDPWrapper(WiFiUDP* wifiUDP) {
      _wifiUDP = wifiUDP;
    };
    
    WiFiUDP* _wifiUDP;
};

// Set up all of the SPI, busy, and reset
// pins used by the Adafruit Airlift/ESP32.
//
void WiFiNetworkHub::setPins(uint8_t spiMOSIPin, uint8_t spiMISOPin, uint8_t spiSCKPin,
    uint8_t spiCSPin, uint8_t resetPin, uint8_t busyPin) {

  // Make sure the right pins are set for SPI
  SPI.setMOSI(spiMOSIPin);
  SPI.setMISO(spiMISOPin);
  SPI.setSCK(spiSCKPin);
  SPI.begin();

  pinMode(busyPin, INPUT);
  pinMode(resetPin, OUTPUT);

  // Make sure pins are set for Wifi
  WiFi.setPins(spiCSPin, busyPin, resetPin, -1);
}

// Starts the network hub specifically using WiFiNINA interface.
//
bool WiFiNetworkHub::begin(const char* ssid, const char* password, Print* printer) {

  printer->print("Found firmware ");
  printer->println(WiFi.firmwareVersion());
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    printer->println("Communication with WiFi module failed!");
    return false;
  }

  if (hasConfiguredLocalIPAddress()) {
    WiFi.config(getConfiguredLocalIPAddress(), getConfiguredDNSIPAddress(),
      getConfiguredGatewayIPAddress(), getConfiguredSubnetMask());
  }
  
  int status = WL_IDLE_STATUS;
  int attemptsLeft = 3;
  
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    if (attemptsLeft == 0) {
      printer->println("All conection attempts exhausted, failed to connected to wifi");
      return false;
    }
    
    printer->print("Attempting to connect to SSID: ");
    printer->println(ssid);
    
    // Connect to WPA/WPA2 network.
    status = WiFi.begin(ssid, password);
    attemptsLeft--;
    
    // wait 10 seconds for connection:
    delay(10000);
  }

  printer->println("Connected to wifi");
  return true;
}

void WiFiNetworkHub::stop(void) {
  WiFi.end();
}

IPAddress WiFiNetworkHub::getLocalIPAddress() {
  return WiFi.localIP();
}

NetworkClient WiFiNetworkHub::getClient() {
  WiFiClient client;
  
  WiFiClientWrapper* clientWrapper = new WiFiClientWrapper(client);
  
  return NetworkFactory::createNetworkClient(clientWrapper);
}

NetworkServer* WiFiNetworkHub::getServer(uint32_t portNum) {
  WiFiServer* tcpServer = new WiFiServer(portNum);
  
  WiFiServerWrapper* serverWrapper = new WiFiServerWrapper(tcpServer);
  
  return NetworkFactory::createNetworkServer(serverWrapper);
}

NetworkUDP* WiFiNetworkHub::getUDP() {
  WiFiUDP* udp = new WiFiUDP();
  
  WiFiUDPWrapper* udpWrapper = new WiFiUDPWrapper(udp);
  
  return NetworkFactory::createNetworkUDP(udpWrapper);
}

void WiFiNetworkHub::printStatus(Print* printer) {
  // print the SSID of the network you're attached to:
  printer->print("SSID: ");
  printer->println(WiFi.SSID());
  
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  printer->print("Signal Strength (RSSI): ");
  printer->print(rssi);
  printer->println(" dBm");

  uint8_t macAddress[6];
  WiFi.macAddress(macAddress);
  printer->print("MAC Address: ");
  for (size_t x = 0; x < sizeof(macAddress); x++) {
    printer->print(macAddress[x], HEX);
    if (x < sizeof(macAddress) - 1) {
      printer->print(":");
    } else {
      printer->println();
    }
  }

  printer->print("IP Address: ");
  printer->println(WiFi.localIP());
  
  printer->print("Subnet Mask: ");
  printer->println(WiFi.subnetMask());
  
  printer->print("Gateway IP: ");
  printer->println(WiFi.gatewayIP());
}

// Static members and methods

WiFiNetworkHub* WiFiNetworkHub::_wifiNetworkHub = NULL;
    
// Returns the instance of WiFiNetworkHub
WiFiNetworkHub WiFiNetworkHub::getInstance() {
  if (_wifiNetworkHub == NULL) {
    _wifiNetworkHub = new WiFiNetworkHub();
  }
  return *_wifiNetworkHub;
};
