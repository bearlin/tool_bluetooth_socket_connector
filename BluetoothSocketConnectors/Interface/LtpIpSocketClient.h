//
// This file contain CLtpIpSocketClient interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPIPSOCKETCLIENT_H_
#define _LTPIPSOCKETCLIENT_H_

#include "LtpIpSocket.h"
#include "ILtpSocketClient.h"

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  
  class CLtpIpSocketClient : public ILtpSocketClient 
  {
  private:
    std::string iInstance;

    int iFdSocketConnect;
    char* iBuffer;
    
    // For test.
    int client_start(int argc, char **argv);
    
  public: 
    CLtpIpSocketClient(std::string instance);
    virtual ~CLtpIpSocketClient();

    //! See base class
    int ClientSocketOpen();

    //! See base class &
    //! Open a connection to server socket at port aPort.  
    //! \param[in]     aAddr        The character string containing an socket server address in IPv4 network dotted-decimal format, "ddd.ddd.ddd.ddd".
    //! \param[in]     aPort        The unsigned short integer hostshort port number of socket server.
    //! \return True if successful, false otherwise
    int ClientSocketConnect(const char* aAddr, unsigned short aPort);

    //! See base class
    int ClientSocketRead(char** aBuffer);

    //! See base class
    int ClientSocketWrite(const char* aBuffer);

    //! See base class
    int ClientSocketClose();

    // For test.
    int test(int argc, const char **argv);
    
  };
  
} //namespace NBsc


#endif  // _LTPIPSOCKETCLIENT_H_