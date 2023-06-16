//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// This is a basic example of using the WiFiNetworkHub
// to connect to a network.

#include <Arduino.h>

#include <WiFiNetworkHub.h>
#include <NetworkServer.h>
#include <NetworkClient.h>

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

WiFiNetworkHub networkHub = WiFiNetworkHub::getWiFiNetworkHub();

uint32_t blinkTime;
int blinkState;

NetworkServer* tcpServer;

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(LED_STATUS_PIN, OUTPUT);
  
  networkHub.setPins(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_CS_PIN, RESET_PIN, BUSY_PIN);

  // Uncomment to give host a fixed ip address, otherwise network assigns via DHCP
  //networkHub.setHostIPAddress(IPAddress(192, 168, 86, 101));

  if (!networkHub.start(SSID, PASSWORD, (Print*)&Serial)) {
    Serial.println("Unable to connect to WiFi network, abort");
    while(true){;}
  }

  networkHub.printStatus((Print*)&Serial);

  // Set up a TCP server on the port
  tcpServer = networkHub.getTCPServer(55555);
  
  blinkTime = millis() + 500;
  blinkState = LOW;
}

void loop() {

  // Check for data sent to the server
  NetworkClient wifiClient = tcpServer->available();
  if (wifiClient) {
    Serial.println("wifiClient returned");
  }
    
  if (millis() >= blinkTime) {
    blinkState = !blinkState;
    digitalWrite(LED_STATUS_PIN, blinkState);
    blinkTime = millis() + 500;
  }
}
