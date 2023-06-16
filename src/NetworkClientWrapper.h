//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKCLIENTWRAPPER_H
#define NETWORKCLIENTWRAPPER_H

#include <Client.h>

// This class defines a wrapper class for Client
// that can be implemented by subclasses to "wrap"
// a specific network implementation. Callers
// should never use this class directly.
//
class NetworkClientWrapper : public Client {
  public:
    virtual ~NetworkClientWrapper(){};
    virtual int connect(IPAddress ip, uint16_t port) = 0;
    virtual int connect(const char *host, uint16_t port) = 0;
    virtual int connectSSL(IPAddress ip, uint16_t port) = 0;
    virtual int connectSSL(const char *host, uint16_t port) = 0;
    virtual size_t write(uint8_t b) = 0;
    virtual size_t write(const uint8_t *buf, size_t size) = 0;
    virtual int available() = 0;
    virtual int read() = 0;
    virtual int read(uint8_t *buf, size_t size) = 0;
    virtual int peek() = 0;
    virtual void flush() = 0;
    virtual void stop() = 0;
    virtual uint8_t connected() = 0;
    virtual operator bool() = 0;
};

class NullNetworkClientWrapper : public NetworkClientWrapper {
  public:
    virtual ~NullNetworkClientWrapper(){};
    virtual int connect(IPAddress ip, uint16_t port) { return 0; };
    virtual int connect(const char *host, uint16_t port) { return 0; };
    virtual int connectSSL(IPAddress ip, uint16_t port) { return 0; };
    virtual int connectSSL(const char *host, uint16_t port) { return 0; };
    virtual size_t write(uint8_t b) { return 0; };
    virtual size_t write(const uint8_t *buf, size_t size) { return 0; };
    virtual int available() { return 0; };
    virtual int read() { return -1; };
    virtual int read(uint8_t *buf, size_t size) { return 0; };
    virtual int peek() { return -1; };
    virtual void flush() {};
    virtual void stop() {};
    virtual uint8_t connected() { return 0; };
    virtual operator bool() { return false; };
};

#endif // NETWORKCLIENTWRAPPER_H
