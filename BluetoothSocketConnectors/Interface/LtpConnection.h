//
// This file contain Connection interface for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPCONNECTION_H_
#define _LTPCONNECTION_H_

#include "LtpCommon.h"
#include "LtpExceptions.h"

// For Generic Socket interface, IP socket and Bluetooth socket.
#include "ILtpSocketClient.h"
#include "ILtpSocketServer.h"
#include "LtpIpSocketClient.h"
#include "LtpIpSocketServer.h"
#include "LtpBluetoothSocketClient.h"
#include "LtpBluetoothSocketServer.h"

#include <iostream>
#include <string>
#include <map>

// For DBG.
// Dummy Connector. Enable this macro to let Connector send/receive messages to itself without any real socket connection.
//#define SOCKET_TEST
  
namespace NBsc //Namespace is BluetoothSocketConnectors
{
  class ILtpConnectionListener 
  {
  public:
    // Caller:        Connection layer (to notify RpcOverSpp layer listener that a message was received).
    // Implement: RpcOverSpp layer.
    virtual void messageReceived(const std::string& message) = 0;
  };
  
  /**
  * In all methods a 'source' parameter is used to indicate the caller.
  * If blockSocketSendMessage() is called by "HU: ", messageReceived() is called on the listener installed as "SP: ". 
  * If blockSocketSendMessage() is called by "SP: ", messageReceived() is called on the listener installed as "HU: ". 
  *
  * A connection typically has 1 client. So there is 'register' listener instead of 'add' listener,
  * which is used in case of multiple clients.
  */
  class ILtpConnection 
  {
  public:
    //! Caller:        RpcOverSpp layer (to register him-self as a listener).
    //! Implement: Connection layer.
    virtual void registerListener(const std::string& source, ILtpConnectionListener& listener) = 0;
    
    //! Caller:        RpcOverSpp layer (to un-register him-self as a listener).
    //! Implement: Connection layer.
    virtual void unregisterListener(const std::string& source) = 0;
    
    //! Caller:        RpcOverSpp layer (to send a message to the other side).
    //! Implement: Connection layer.
    virtual void blockSocketSendMessage(const std::string& source, const std::string aMessage) = 0;

    //! Caller:        Connection or RpcOverSpp layer (to wait and receive a message from the other side).
    //! Implement: Connection layer.
    //! \return True if successful, false otherwise
    virtual int blockSocketReceiveMessage() = 0;

    // ----------------------------------------------------------------------------------------------------------------
    // HU Socket Server Design ideas:
    // 
    // [Requirements]
    // 1. Requests shall be queued while there is request still in process on HU.
    // 2. Requests shall be handled by receiving order.
    // 3. At current stage we assume that there is only one SP will connect to the socket server, so we will accept only one connection at the same time.
    //    This also means that we can use a iterative server to simplify the server design.
    // 
    // [Current Design]
    // 1. [HU] After accept one connection: 
    //         Main thread will keep reading messages from socket in a infinite loop CLtpConnection::startConnectorSocketServerLoop(). 
    // 2. [HU] A message pool(sgMsgPool) which will be shared between below two threads: 
    //         addOneMessageToPool thread(Lock mutex, add message to pool, then signal MessagePoolWorker) and 
    //         MessagePoolWorker thread(Lock mutex and sgCondVarMsgPool signal, and get all messages from pool one by one, decode them and send RESP to socket client).
    // 3. [HU] Threads controlled by below two pthread variables:
    //         sgMutexMsgPool: for lock the sgMsgPool shared variable.
    //         sgCondVarMsgPool: for addOneMessageToPool thread to signal the waiting MessagePoolWorker thread that the sgMsgPool.size() > 0.
    // 4. [HU] The order of request messages in sgMsgPool don't need to sync with its RequestId number.
    // 
    // [TO DO]
    // 1. [HU] MessagePoolWorker thread have to get the queued request element in the order of its RequestId number.
    //----------------------------------------------------------------------------------------------------------------
    //! Caller:     RpcOverSpp layer (Tell SocketServer entering Accept() -> Read()/Write() loop).
    //! \param[in]  aMaxLoopCount    The maximum loop count. NOTE: Input 0 to start a infinity loop.
    //! Implement:  Connection layer.
    virtual bool startConnectorSocketServerLoop(unsigned int aMaxLoopCount) = 0;
  };

  class CLtpConnection : public ILtpConnection 
  {
  private:
    std::string iInstance;
    // If I am HU, the iHuListener will be registered and iSpListener will be NULL.
    ILtpConnectionListener* iSpListener; // listener for source "SP: ".
    ILtpConnectionListener* iHuListener; // listener for source "HU: ".

    ILtpSocketServer* iSocketServer; //Generic Socket channel for "HU: ".
    ILtpSocketClient* iSocketClient; //Generic Socket channel for "SP: ".

    TSocketType iSocketType; // ESocketType_IP: IpSocket; ESocketType_BT: BluetoothSocket.
    
    CLtpBluetoothSocketServer* iBTSocketServer; //Bluetooth Socket channel for "HU: ".
    CLtpBluetoothSocketClient* iBTSocketClient; //Bluetooth Socket channel for "SP: ".
    
    CLtpIpSocketServer* iIpSocketServer; //IP Socket channel for "HU: ".
    CLtpIpSocketClient* iIpSocketClient; //IP Socket channel for "SP: ".

    // This is a buffer for R/W messages.
    std::string iMessageBuffer;
    char* iMsgBuffer;

  public: 
    CLtpConnection(std::string instance, TSocketType aSocketType);
    virtual ~CLtpConnection();

  #ifdef SOCKET_TEST
    int socketTest(std::string instance);
  #endif

    //! See base class
    void blockSocketSendMessage(const std::string& source, const std::string aMessage);
  
    //! See base class
    int blockSocketReceiveMessage();

    //! See base class
    bool startConnectorSocketServerLoop(unsigned int aMaxLoopCount);

    //! See base class
    void registerListener(const std::string& source, ILtpConnectionListener& listener);

    //! See base class
    void unregisterListener(const std::string& source);

    //! HU main thread should only be blocked by ServerSocketRead().
    //! This function will create a addOneMessageToPool thread for "add one request to message pool" 
    //! to prevent block by message pool mutex.
    static void* addOneMessageToPool(void* aMsg);

    //! HU main thread should only be blocked by ServerSocketRead().
    //! This function will create a MessagePoolWorker thread for "retrive and remove one request 
    //! from message pool, then decode this message with protocol layers" to prevent block by message pool mutex.
    static void* MessagePoolWorker(void* unuse);
  };
} //namespace NBsc


#endif  // _LTPCONNECTION_H_
