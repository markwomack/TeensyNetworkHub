//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#include <Arduino.h>

#include <WiFiNetworkHub.h>

const uint8_t BUSY_PIN(8);
const uint8_t RESET_PIN(9);
const uint8_t SPI_CS_PIN(10);
const uint8_t SPI_MOSI_PIN(11);
const uint8_t SPI_MISO_PIN(12);
const uint8_t SPI_SCK_PIN(13);
const uint8_t LED_STATUS_PIN(14); // LED that is used to indicate status/idle

const char SSID[]("Your WiFi SSID Here");
const char PASSWORD[]("Your WiFi Password Here");

WiFiNetworkHub networkHub;

uint32_t blinkTime;
int blinkState;

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(LED_STATUS_PIN, OUTPUT);
  
  networkHub.setPins(SPI_MOSI_PIN, SPI_MISO_PIN, SPI_SCK_PIN, SPI_CS_PIN, RESET_PIN, BUSY_PIN);

  // Uncomment to give host a fixed ip address, otherwise network assigns
  //networkHub.setHostIPAddress(IPAddress(192, 168, 100, 101));

  if (!networkHub.start(SSID, PASSWORD, (Print*)&Serial)) {
    Serial.println("Unable to connect to WiFi network, abort");
    while(true){;}
  }

  networkHub.printWiFiStatus((Print*)&Serial);
  
  blinkTime = millis() + 500;
  blinkState = LOW;
}

void loop() {
  if (millis() >= blinkTime) {
    blinkState = !blinkState;
    digitalWrite(LED_STATUS_PIN, blinkState);
    blinkTime = millis() + 500;
  }
}
