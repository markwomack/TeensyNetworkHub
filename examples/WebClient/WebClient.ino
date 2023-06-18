//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

/*
  Web client

 This sketch connects to a website (http://www.google.com)
 and downloads the page contents.

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen
 modified 17 Jun 2023
 by Mark Womack to use the TeensyNetworkHub library

 */

// See this file for implementation spcific settings
#include "connect_network_hub.h"

//***** ALL OF THE CODE BELOW HERE IS COMMON AND NETWORK AGNOSTIC

// How long to wait until deciding all data has been sent
#define IDLE_THRESHOLD_MICROS 1000000  // 1 second

// if you don't want to use DNS use the numeric IP instead
// of the name for the server:
//IPAddress server(142,251,40,132);  // numeric IP for Google (no DNS)
char server[] = "www.google.com";    // name address for Google (using DNS)

// Get a NetworkClient from the network hub.
NetworkClient client = networkHub.getClient();

// Variables to measure the speed
uint32_t beginMicros, lastReadTime;
uint32_t byteCount = 0;
bool printWebData = true;  // set to false for better speed measurement

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // Connect the network hub
  connectNetworkHub();

  // Print the status of the network hub
  networkHub.printStatus((Print*)&Serial);
  
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  beginMicros = micros();
  lastReadTime = beginMicros;
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  int len = client.available();
  if (len > 0) {
    byte buffer[80];
    if (len > 80) len = 80;
    client.read(buffer, len);
    lastReadTime = micros();
    if (printWebData) {
      Serial.write(buffer, len); // show in the serial monitor (slows some boards)
    }
    byteCount = byteCount + len;
  }

  // if the server's disconnected, stop the client:
  if (!client.connected() || micros() >= (lastReadTime + IDLE_THRESHOLD_MICROS)) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    Serial.print("Received ");
    Serial.print(byteCount);
    Serial.print(" bytes in ");
    float seconds = (float)(lastReadTime - beginMicros) / 1000000.0;
    Serial.print(seconds, 4);
    float rate = (float)byteCount / seconds / 1000.0;
    Serial.print(", rate = ");
    Serial.print(rate);
    Serial.print(" kbytes/second");
    Serial.println();

    // do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
}
