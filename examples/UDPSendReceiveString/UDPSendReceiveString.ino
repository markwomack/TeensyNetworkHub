//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

/*
 UDPSendReceiveString:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender

 On linux you can use to command
 
   netcat -u host port
   
 to send data to the UDP port this program will open and monitor.

 This code is in the public domain.

 created 21 Aug 2010
 by Michael Margolis
 modified 17 Jun 2023
 by Mark Womack to use the TeensyNetworkHub library

 */

// See this file for implementation spcific settings
#include "connect_network_hub.h"

//***** ALL OF THE CODE BELOW HERE IS COMMON AND NETWORK AGNOSTIC

#define UDP_TX_PACKET_MAX_SIZE 256

uint16_t localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  // buffer to hold incoming packet,
char replyBuffer[] = "acknowledged\n";        // a string to send back

// An NetworkUDP instance to let us send and receive packets over UDP
NetworkUDP* udp = networkHub.getUDP();

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Network UDPSendAndReceive Example");

  // Connect the network hub
  connectNetworkHub();

  // Print the status of the network hub
  networkHub.printStatus((Print*)&Serial);

  // start UDP
  udp->begin(localPort);
  Serial.println();
  Serial.print("UDP is at ");
  Serial.print(networkHub.getLocalIPAddress());
  Serial.print(" ");
  Serial.println(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = udp->parsePacket();
  if (packetSize > 0) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = udp->remoteIP();
    for (int i=0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(udp->remotePort());

    // read the packet into packetBufffer
    size_t size = udp->read(packetBuffer, UDP_TX_PACKET_MAX_SIZE-1);
    packetBuffer[size] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply to the IP address and port that sent us the packet we received
    udp->beginPacket(udp->remoteIP(), udp->remotePort());
    udp->write((uint8_t*)replyBuffer, sizeof(replyBuffer));
    udp->endPacket();
  }
  delay(10);
}
