//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// This include file contains all of the network specific
// code for setting up the network hub. You probably would
// not use it this way in your own code, instead choosing one
// implementation to use. But organizing it into a single
// file or class is a good practice so you can easily swap the
// implementation as needed.

#ifndef CONNECT_NETWORK_HUB_H
#define CONNECT_NETWORK_HUB_H

//***** UNCOMMENT one of these to use a specific hub type
//#define WIFI_NINA_NETWORK_HUB
#define QNETHERNET_NETWORK_HUB
//#define NATIVE_ETHERNET_NETWORK_HUB

#if defined(QNETHERNET_NETWORK_HUB)

#include <QNEthernetNetworkHub.h>
QNEthernetNetworkHub networkHub = QNEthernetNetworkHub::getInstance();

#elif defined(WIFI_NINA_NETWORK_HUB)

#include <WiFiNINANetworkHub.h>
WiFiNINANetworkHub networkHub = WiFiNINANetworkHub::getInstance();

// This is required for the WiFiNetwork Hub

// Pins used in example. It is a simple
// circuit with the Teensy attached to the
// Adafruit Airlift (or equivalent ESP32) and
// a status LED on pin 14.
const uint8_t BUSY_PIN(8);
const uint8_t RESET_PIN(9);
const uint8_t SPI_CS_PIN(10);
const uint8_t SPI_MOSI_PIN(11);
const uint8_t SPI_MISO_PIN(12);
const uint8_t SPI_SCK_PIN(13);
const uint8_t LED_STATUS_PIN(14); // LED that is used to indicate status/idle

const char SSID[]("<SSID OF YOUR WIFI HERE>");
const char PASSWORD[]("<PASSWORD OF YOUR WIFI HERE>");

#elif defined(NATIVE_ETHERNET_NETWORK_HUB)

#include <NativeEthernetNetworkHub.h>
NativeEthernetNetworkHub networkHub = NativeEthernetNetworkHub::getInstance();

// This is required for the EthernetNetowrkHub

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

#endif

// The fixed IP address instead of using DHCP
const IPAddress localIP(192, 168, 86, 101);

void connectNetworkHub() {

  Serial.println("Starting the network hub...");

  // Uncomment to give host a fixed ip address, otherwise network assigns via DHCP
  //networkHub.setLocalIPAddress(localIP);
  
#if defined(QNETHERNET_NETWORK_HUB)

if (!networkHub.begin((Print*)&Serial)) {
    Serial.println("Network not found, aborting");
    while (true) {
      delay(1); // do nothing, no point running without the netowrk
    }
  }

#elif defined(WIFI_NINA_NETWORK_HUB)

  networkHub.setPins(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_CS_PIN, RESET_PIN, BUSY_PIN);
  
  if (!networkHub.begin(SSID, PASSWORD, (Print*)&Serial)) {
    Serial.println("Network not found, aborting");
    while (true) {
      delay(1); // do nothing, no point running without the netowrk
    }
  }

#elif defined(NATIVE_ETHERNET_NETWORK_HUB)

  if (!networkHub.begin(mac, (Print*)&Serial)) {
    Serial.println("Network not found, aborting");
    while (true) {
      delay(1); // do nothing, no point running without the network
    }
  }

#endif

}

#endif // CONNECT_NETWORK_HUB_H
