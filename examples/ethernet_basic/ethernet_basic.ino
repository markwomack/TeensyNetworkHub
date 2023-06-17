//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

// This is a basic example of using the EthernetNetworkHub
// to connect to a network.

#include <Arduino.h>

#include <EthernetNetworkHub.h>
#include <NetworkServer.h>
#include <NetworkClient.h>

const uint8_t LED_STATUS_PIN(13); // LED that is used to indicate status/idle

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 86, 101);

EthernetNetworkHub networkHub = EthernetNetworkHub::getInstance();

uint32_t blinkTime;
int blinkState;

NetworkServer* tcpServer;

void setup() {
  Serial.begin(9600);
  delay(500);

  pinMode(LED_STATUS_PIN, OUTPUT);

  // Uncomment to give host a fixed ip address, otherwise network assigns via DHCP
  //networkHub.setHostIPAddress(ip);

  if (!networkHub.begin(mac, (Print*)&Serial)) {
    Serial.println("Unable to start ethernet network, abort");
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
  NetworkClient client = tcpServer->available();
  if (client) {
    Serial.println("NetworkClient returned");
  }
    
  if (millis() >= blinkTime) {
    blinkState = !blinkState;
    digitalWrite(LED_STATUS_PIN, blinkState);
    blinkTime = millis() + 500;
  }
}
