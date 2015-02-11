//
// This file contain common IP Socket interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPIPSOCKET_H_
#define _LTPIPSOCKET_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h> 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <time.h> 

//#define SEND_TO_CLIENT //Enable/Disable this macro will send from server/client to client/server.
#define SOCKET_PORT (3322)
#define SOCKET_CONN_BACKLOG (1)//(3)
#define SOCKET_SERVER_ADDR "127.0.0.1"

namespace NBsc //Namespace is BluetoothSocketConnectors
{

  // TODO.

} //namespace NBsc


#endif  // _LTPIPSOCKET_H_