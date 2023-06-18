//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef CONNECT_NETWORK_HUB_H
#define CONNECT_NETWORK_HUB_H

#ifdef ETHERNET_NETWORK_HUB

// This is required for the EthernetNetowrkHub

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#else

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
#endif

// Set the static IP address instead of using DHCP
const IPAddress localIP(192, 168, 86, 101);

void connectNetworkHub() {

  Serial.println("Starting the network hub...");

  // Uncomment to give host a fixed ip address, otherwise network assigns via DHCP
  //networkHub.setLocalIPAddress(localIP);
  
#ifdef ETHERNET_NETWORK_HUB

  if (!networkHub.begin(mac, (Print*)&Serial)) {
    Serial.println("Network not found, aborting");
    while (true) {
      delay(1); // do nothing, no point running without the network
    }
  }
  
#else

  networkHub.setPins(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_CS_PIN, RESET_PIN, BUSY_PIN);
  
  if (!networkHub.begin(SSID, PASSWORD, (Print*)&Serial)) {
    Serial.println("Network not found, aborting");
    while (true) {
      delay(1); // do nothing, no point running without the netowrk
    }
  }
  
#endif

}

#endif // CONNECT_NETWORK_HUB_H
