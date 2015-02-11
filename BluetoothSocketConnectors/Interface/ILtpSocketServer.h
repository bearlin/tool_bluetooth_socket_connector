//
// This file contain SocketServer interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _ILTPSOCKETSERVER_H_
#define _ILTPSOCKETSERVER_H_

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  class ILtpSocketServer
  {
  public:    
    //! Open a listenning server socket(iFdSocketListen).  
    //! \param[in]     aPort      The port number of socket server.
    //! \param[in]     aBacklog   The maximum length to which the queue of pending connections for iFdSocketListen may grow.
    //! \return True if successful, false otherwise
    virtual int ServerSocketOpen(unsigned short aPort, int aBacklog) = 0;

    //! Accept a connection on a socket. 
    //! It extracts the first connection request on the queue of pending connections for the listening socket(iFdSocketListen),
    //! creates a new connected socket, and set a new file descriptor(iFdSocketConnect) referring to that socket.  
    //! The newly created socket is not in the listening state.  
    //! The original socket iFdSocketListen is unaffected by this call.
    //! It blocks the caller until a connection is present. 
    //! \return True if successful, false otherwise
    virtual int ServerSocketAccept() = 0;

    //! Read from socket file descriptor(iFdSocketConnect) to iBuffer.  
    //! \param[out] aBuffer   The pointer to the buffer after data read.
    //! \return True if successful, false otherwise
    virtual int ServerSocketRead(char** aBuffer) = 0;

    //! Write to socket file descriptor(iFdSocketConnect) to iBuffer, the aBuffer should be a character string.  
    //! \param[in]     aBuffer       The pointer to the buffer before data sent. NOTE: If aBuffer = NULL, a timestamp will be sent.
    //! \return True if successful, false otherwise
    virtual int ServerSocketWrite(const char* aBuffer) = 0;

    //! Close the server socket(iFdSocketListen and iFdSocketConnect).  
    //! \return True if successful, false otherwise
    virtual int ServerSocketClose() = 0;
  };
  
} //namespace NBsc


#endif  // _ILTPSOCKETSERVER_H_
