//
// This file contain SocketClient interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _ILTPSOCKETCLIENT_H_
#define _ILTPSOCKETCLIENT_H_

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  class ILtpSocketClient
  {
  public:
    //! Open a client socket(iFdSocketConnect).  
    //! \return True if successful, false otherwise
    virtual int ClientSocketOpen() = 0;

    //! Open a connection to server socket at port aPort.  
    //! \param[in]     aAddr        The character string containing an socket server address.
    //! \param[in]     aPort        The port number of socket server.
    //! \return True if successful, false otherwise
    virtual int ClientSocketConnect(const char* aAddr, unsigned short aPort) = 0;

    //! Read from socket file descriptor(iFdSocketConnect) to iBuffer.  
    //! \param[out] aBuffer   The pointer to the buffer after data read.
    //! \return True if successful, false otherwise
    virtual int ClientSocketRead(char** aBuffer) = 0;

    //! Write to socket file descriptor(iFdSocketConnect) to iBuffer, the aBuffer should be a character string.  
    //! \param[in]     aBuffer       The pointer to the buffer before data sent. NOTE: If aBuffer = NULL, a timestamp will be sent.
    //! \return True if successful, false otherwise
    virtual int ClientSocketWrite(const char* aBuffer) = 0;

    //! Close the client socket(iFdSocketConnect). 
    //! \return True if successful, false otherwise
    virtual int ClientSocketClose() = 0;
  };
  
} //namespace NBsc


#endif  // _ILTPSOCKETCLIENT_H_