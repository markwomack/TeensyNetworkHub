//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKSERVER_H
#define NETWORKSERVER_H

#include <Server.h>
#include "NetworkServerWrapper.h"
#include "NetworkClient.h"

// A network server that will listen and write
// to specific port on the hub. Instances of
// NetworkServer are created by calls to the
// NetworkHub.
//
class NetworkServer : public Print {
  public:
    NetworkClient available() { return _serverWrapper->available(); };
    void begin() { _serverWrapper->begin(); };
    size_t write(uint8_t b) { return _serverWrapper->write(b); };
    size_t write(const uint8_t *buf, size_t size) { return _serverWrapper->write(buf, size); };
    
    ~NetworkServer() {
      delete _serverWrapper;
    };
    
  protected:
    NetworkServerWrapper* _serverWrapper;
    
  private:
    friend class NetworkFactory;
    
    NetworkServer() {};
    NetworkServer(NetworkServerWrapper* serverWrapper) {
      _serverWrapper = serverWrapper;
    };
};

#endif // NETWORKSERVER_H
