//
// This file contain CLtpBluetoothSocketServer interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPBLUETOOTHSOCKETSERVER_H_
#define _LTPBLUETOOTHSOCKETSERVER_H_

#include "LtpBluetoothSocket.h"
#include "ILtpSocketServer.h"

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  
  class CLtpBluetoothSocketServer : public ILtpSocketServer 
  {
  private:
    std::string iInstance;

    int iFdSocketListen;
    int iFdSocketConnect;
    char* iBuffer;

    // SDP service session.
    sdp_session_t* iSession;

    // For test.
    sdp_session_t* register_service(int port);
    int server_start(int argc, char **argv);
    
  public: 
    CLtpBluetoothSocketServer(std::string instance);
    virtual ~CLtpBluetoothSocketServer();

    //! See base class &
    //! Open a listenning server socket(iFdSocketListen).  
    //! \param[in]     aPort      The unsigned short integer hostshort port number of socket server. NOTE: Input 0 to start dynamic port assignment.
    //! \param[in]     aBacklog   The maximum length to which the queue of pending connections for iFdSocketListen may grow.
    //! \return True if successful, false otherwise
    int ServerSocketOpen(unsigned short aPort, int aBacklog);

    //! See base class
    int ServerSocketAccept();

    //! See base class
    int ServerSocketRead(char** aBuffer);

    //! See base class
    int ServerSocketWrite(const char* aBuffer);

    //! See base class
    int ServerSocketClose();

    // For test.
    int test(int argc, char **argv);
    
  };
  
} //namespace NBsc


#endif  // _LTPBLUETOOTHSOCKETSERVER_H_
