//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKUDPWRAPPER_H
#define NETWORKUDPWRAPPER_H

#include <Udp.h>

// This class defines a wrapper class for UDP
// that can be implemented by subclasses to "wrap"
// a specific network implementation. Callers
// should never use this class directly.
//
class NetworkUDPWrapper : public UDP {
  public:
    virtual ~NetworkUDPWrapper(){};
    // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
    virtual uint8_t begin(uint16_t p) = 0;
    // initialize, start listening on specified multicast IP address and port. Returns 1 if successful, 0 on failure
    virtual uint8_t beginMulticast(IPAddress ip, uint16_t p) = 0;
    // Finish with the UDP socket
    virtual void stop() = 0;

    // Sending UDP packets
  
    // Start building up a packet to send to the remote host specific in ip and port
    // Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
    virtual int beginPacket(IPAddress ip, uint16_t port) = 0;
    // Start building up a packet to send to the remote host specific in host and port
    // Returns 1 if successful, 0 if there was a problem resolving the hostname or port
    virtual int beginPacket(const char *host, uint16_t port) = 0;
    // Finish off this packet and send it
    // Returns 1 if the packet was sent successfully, 0 if there was an error
    virtual int endPacket() = 0;
    // Write a single byte into the packet
    virtual size_t write(uint8_t b) = 0;
    // Write size bytes from buffer into the packet
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;

    // Start processing the next available incoming packet
    // Returns the size of the packet in bytes, or 0 if no packets are available
    virtual int parsePacket() = 0;
    // Number of bytes remaining in the current packet
    virtual int available() = 0;
    // Read a single byte from the current packet
    virtual int read() = 0;
    // Read up to len bytes from the current packet and place them into buffer
    // Returns the number of bytes read, or 0 if none are available
    virtual int read(unsigned char* buffer, size_t len) = 0;
    // Read up to len characters from the current packet and place them into buffer
    // Returns the number of characters read, or 0 if none are available
    virtual int read(char* buffer, size_t len) = 0;
    // Return the next byte from the current packet without moving on to the next byte
    virtual int peek() = 0;
    // Finish reading the current packet
    virtual void flush() = 0;

    // Return the IP address of the host who sent the current incoming packet
    virtual IPAddress remoteIP() = 0;
    // Return the port of the host who sent the current incoming packet
    virtual uint16_t remotePort() = 0;
};

#endif // NETWORKUDPWRAPPER_H
    
