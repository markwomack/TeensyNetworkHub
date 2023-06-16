//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKUDP_H
#define NETWORKUDP_H

#include <Udp.h>
#include "NetworkUDPWrapper.h"

// A class that will send data via the UDP
// protocol from a port on the hub to a 
// remote ip address and port. Instances of
// NetworkUDP are created by calls to
// NetworkHub.
//
class NetworkUDP : public UDP {
  public:
    
    // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
    uint8_t begin(uint16_t p) { return _udpWrapper->begin(p); };
    // initialize, start listening on specified multicast IP address and port. Returns 1 if successful, 0 on failure
    uint8_t beginMulticast(IPAddress ip, uint16_t p) { return _udpWrapper->beginMulticast(ip, p); };
    // Finish with the UDP socket
    void stop() { _udpWrapper->stop(); };

    // Sending UDP packets
  
    // Start building up a packet to send to the remote host specific in ip and port
    // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
    int beginPacket(IPAddress ip, uint16_t port) { return _udpWrapper->beginPacket(ip, port); };
    // Start building up a packet to send to the remote host specific in host and port
    // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
    int beginPacket(const char *host, uint16_t port) { return _udpWrapper->beginPacket(host, port); };
    // Finish off this packet and send it
    // Returns 1 if the packet was sent successfully, 0 if there was an error
    int endPacket() { return _udpWrapper->endPacket(); };
    // Write a single byte into the packet
    size_t write(uint8_t b) { return _udpWrapper->write(b); };
    // Write size bytes from buffer into the packet
    size_t write(const uint8_t *buffer, size_t size) { return _udpWrapper->write(buffer, size); };

    // Start processing the next available incoming packet
    // Returns the size of the packet in bytes, or 0 if no packets are available
    int parsePacket() { return _udpWrapper->parsePacket(); };
    // Number of bytes remaining in the current packet
    int available() { return _udpWrapper->available(); };
    // Read a single byte from the current packet
    int read() { return _udpWrapper->read(); };
    // Read up to len bytes from the current packet and place them into buffer
    // Returns the number of bytes read, or 0 if none are available
    int read(unsigned char* buffer, size_t len) { return _udpWrapper->read(buffer, len); };
    // Read up to len characters from the current packet and place them into buffer
    // Returns the number of characters read, or 0 if none are available
    int read(char* buffer, size_t len) { return _udpWrapper->read(buffer, len); };
    // Return the next byte from the current packet without moving on to the next byte
    int peek() { return _udpWrapper->peek(); };
    void flush() { _udpWrapper->flush(); };	// Finish reading the current packet

    // Return the IP address of the host who sent the current incoming packet
    IPAddress remoteIP() { return _udpWrapper->remoteIP(); };
    // Return the port of the host who sent the current incoming packet
    uint16_t remotePort() { return _udpWrapper->remotePort(); };
      
    ~NetworkUDP() {
      delete _udpWrapper;
    };
    
  protected:
    NetworkUDPWrapper* _udpWrapper;
      
  private:
    friend class NetworkFactory;
      
    NetworkUDP() {};
    NetworkUDP(NetworkUDPWrapper* udpWrapper) {
      _udpWrapper = udpWrapper;
    };
};
#endif // 

