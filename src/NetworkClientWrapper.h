//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKCLIENTWRAPPER_H
#define NETWORKCLIENTWRAPPER_H

#include <DebugMsgs.h>
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
    virtual IPAddress remoteIP() = 0;
    virtual uint16_t remotePort() = 0;
    
    virtual NetworkClientWrapper* clone() = 0;
};

// This is a 'null' NetworkClientWrapper that is used to allow
// the creation of stack instances of NetworkClient. It should
// not be used generally for anything else.
class NullNetworkClientWrapper : public NetworkClientWrapper {
  public:
    ~NullNetworkClientWrapper(){};
    int connect(IPAddress ip, uint16_t port) { return 0; };
    int connect(const char *host, uint16_t port) { return 0; };
    size_t write(uint8_t b) { return 0; };
    size_t write(const uint8_t *buf, size_t size) { return 0; };
    int available() { return 0; };
    int read() { /*DebugMsgs.debug().println("In NullNetworkClientWrapper read").flush();*/ return -1; };
    int read(uint8_t *buf, size_t size) { return 0; };
    int peek() { return -1; };
    void flush() {};
    void stop() {};
    uint8_t connected() { return 0; };
    operator bool() { return false; };
    IPAddress remoteIP() { return IPAddress(0,0,0,0); };
    uint16_t remotePort() { return 0; };
    NetworkClientWrapper* clone() {
      return new NullNetworkClientWrapper();
    }
};

#endif // NETWORKCLIENTWRAPPER_H
