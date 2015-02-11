//
// This file contain common Bluetooth interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPBLUETOOTHSOCKET_H_
#define _LTPBLUETOOTHSOCKET_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <errno.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

#define BT_SOCKET_MIN_PORT (1)
#define BT_SOCKET_MAX_PORT (30)
//#define BT_SOCKET_FIXED_PORT (11)

#define BT_SOCKET_CONN_BACKLOG (1)//(3)
#define BT_SOCKET_SERVER_ADDR "1C:4B:D6:0F:F1:56" //BuildServerTest-linstNB

namespace NBsc //Namespace is BluetoothSocketConnectors
{

  // TODO.

} //namespace NBsc


#endif  // _LTPBLUETOOTHSOCKET_H_