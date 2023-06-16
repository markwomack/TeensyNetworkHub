//
// Licensed under the MIT license.
// See accompanying LICENSE file for details.
//

#ifndef NETWORKFACTORY_H
#define NETWORKFACTORY_H

#include "NetworkClient.h"
#include "NetworkClientWrapper.h"
#include "NetworkUDP.h"
#include "NetworkUDPWrapper.h"
#include "NetworkServer.h"
#include "NetworkServerWrapper.h"

// This class implements factory methods to create
// the primary objects returned by the NetworkHub.
//
class NetworkFactory {
  public:
    static NetworkClient createNetworkClient(NetworkClientWrapper* clientWrapper) {
      return NetworkClient(clientWrapper);
    };
    
    static NetworkUDP* createNetworkUDP(NetworkUDPWrapper* udpWrapper) {
      return new NetworkUDP(udpWrapper);
    };
    
    static NetworkServer* createNetworkServer(NetworkServerWrapper* serverWrapper) {
      return new NetworkServer(serverWrapper);
    };
  
  private:
    NetworkFactory(){};
};
#endif // NETWORKFACTORY_H
