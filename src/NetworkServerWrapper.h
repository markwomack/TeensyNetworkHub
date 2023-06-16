//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKSERVERWRAPPER_H
#define NETWORKSERVERWRAPPER_H

#include <Server.h>
#include "NetworkClient.h"

// This class defines a wrapper class for Server
// that can be implemented by subclasses to "wrap"
// a specific network implementation. Callers
// should never use this class directly.
//
class NetworkServerWrapper : public Print {
  public:
    virtual ~NetworkServerWrapper(){};
    virtual NetworkClient available() = 0;
    virtual void begin() = 0;
    virtual size_t write(uint8_t b) = 0;
    virtual size_t write(const uint8_t *buf, size_t size) = 0;
};

#endif // NETWORKSERVERWRAPPER_H
