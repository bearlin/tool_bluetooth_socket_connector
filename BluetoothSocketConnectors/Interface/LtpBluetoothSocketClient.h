//
// This file contain CLtpBluetoothSocketClient interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPBLUETOOTHSOCKETCLIENT_H_
#define _LTPBLUETOOTHSOCKETCLIENT_H_

#include "LtpBluetoothSocket.h"
#include "ILtpSocketClient.h"

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  
  class CLtpBluetoothSocketClient : public ILtpSocketClient 
  {
  private:
    std::string iInstance;

    int iFdSocketConnect;
    char* iBuffer;
    
    // For test.
    int search_service(const char *bt_addr);
    int client_start(int argc, char **argv);
    
  public: 
    CLtpBluetoothSocketClient(std::string instance);
    virtual ~CLtpBluetoothSocketClient();

    //! See base class
    int ClientSocketOpen();

    //! See base class &
    //! Open a connection to server socket at port aPort.  
    //! \param[in]     aAddr        The character string containing an socket server address in Bluetooth MAC dotted-hex format, "XX.XX.XX.XX".
    //! \param[in]     aPort        The unsigned short integer hostshort port number of socket server. NOTE: Input 0 to start dynamic port assignment returned by search_service().
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


#endif  // _LTPBLUETOOTHSOCKETCLIENT_H_