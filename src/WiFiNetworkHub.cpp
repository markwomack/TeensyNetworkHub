//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

//
// This is an implementation of a simple network hub based on
// a WiFi connection. This version requires the Adafruit version
// of the Arduino WiFiNINA library since the SPI pins are specified.
//

// Third-party includes
#include <SPI.h>
#include <WiFiNINA.h> // https://github.com/arduino-libraries/WiFiNINA
#include <WiFiUdp.h>
#include <WiFiServer.h>

// Local includes
#include "WiFiNetworkHub.h"

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

void WiFiNetworkHub::setHostIPAddress(IPAddress hostIPAddress) {
  // Set a static ip address
  WiFi.config(hostIPAddress);
}

bool WiFiNetworkHub::start(const char* ssid, const char* password, Print* printer) {

  printer->print("Found firmware ");
  printer->println(WiFi.firmwareVersion());
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    printer->println("Communication with WiFi module failed!");
    return false;
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

UDP* WiFiNetworkHub::getUdpPort(uint32_t portNum) {
  WiFiUDP* udp = new WiFiUDP();
  udp->begin(portNum);
  return udp;
}

WiFiServer* WiFiNetworkHub::getTCPServer(uint32_t portNum) {
  WiFiServer* tcpServer = new WiFiServer(portNum);
  tcpServer->begin();
  return tcpServer;
}

void WiFiNetworkHub::printWiFiStatus(Print* printer) {
  // print the SSID of the network you're attached to:
  printer->print("SSID: ");
  printer->println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  printer->print("IP Address: ");
  printer->println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  printer->print("signal strength (RSSI):");
  printer->print(rssi);
  printer->println(" dBm");
}
