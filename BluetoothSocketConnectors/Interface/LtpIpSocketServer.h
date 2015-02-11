//
// This file contain CLtpIpSocketServer interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPIPSOCKETSERVER_H_
#define _LTPIPSOCKETSERVER_H_

#include "LtpIpSocket.h"
#include "ILtpSocketServer.h"

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  
  class CLtpIpSocketServer : public ILtpSocketServer 
  {
  private:
    std::string iInstance;

    int iFdSocketListen;
    int iFdSocketConnect;
    char* iBuffer;
    
    // For test.
    int server_start(int argc, char **argv);
    
  public: 
    CLtpIpSocketServer(std::string instance);
    virtual ~CLtpIpSocketServer();

    //! See base class &
    //! Open a listenning server socket(iFdSocketListen).  
    //! \param[in]     aPort      The unsigned short integer hostshort port number of socket server.
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


#endif  // _LTPIPSOCKETSERVER_H_