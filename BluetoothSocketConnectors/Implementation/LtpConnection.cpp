//
// This file contain Connection implementation for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpConnection.h"
#include "ILogging.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <stdio.h>

using namespace NBsc;

logging::TLogComponent iLtpLogId;

// Message pool.
static int sgMsgSerialNum; // Just for std::map mapping.
static std::map<int, std::string> sgMsgPool; // Is there better data structure for our application?
static ILtpConnectionListener* sgHuListener = NULL;

// Mutex and CondVar for better message pool and threads control.
static CPthreadMutex sgMutexMsgPool;
static CPthreadCondVar sgCondVarMsgPool(sgMutexMsgPool);

CLtpConnection::CLtpConnection(std::string instance, TSocketType aSocketType) : 
  iInstance(instance),
  iSpListener(NULL),
  iHuListener(NULL),
  iSocketServer(NULL),
  iSocketClient(NULL),
  iSocketType(ESocketType_IP),
  iBTSocketServer(NULL),
  iBTSocketClient(NULL),
  iIpSocketServer(NULL),
  iIpSocketClient(NULL), 
  iMessageBuffer(""), 
  iMsgBuffer(NULL)
{
  logging::IControl().RegisterComponent( "CLtpConnection", iLtpLogId);
  LogInterface( iLtpLogId, "");

  iSocketType = aSocketType;
  
#ifdef SOCKET_TEST
  socketTest(instance);
  return;
#endif //SOCKET_TEST

  // Init message pool.
  sgMsgSerialNum = 0;
  sgMsgPool.clear();
  LogInfo( iLtpLogId, " I am "<<instance.c_str());
  if (instance == std::string("HU: "))
  {
    // Create a SocketServer instance.
    if (ESocketType_BT == iSocketType)
    {
      LogInfo( iLtpLogId, "Create BluetoothSocketServer...");
      iBTSocketServer = new CLtpBluetoothSocketServer(instance);
      iSocketServer = iBTSocketServer;
    }
    else
    {
      LogInfo( iLtpLogId, "Create IPSocketServer...");
      iIpSocketServer = new CLtpIpSocketServer(instance);
      iSocketServer = iIpSocketServer;
    }
    
    // Open a listenning socket.
    unsigned short Port;
    int Backlog;
    if (ESocketType_BT == iSocketType)
    {
      Port = 0; //Input 0 to start dynamic port assignment.
      Backlog = BT_SOCKET_CONN_BACKLOG;
    }
    else
    {
      Port = SOCKET_PORT;
      Backlog = SOCKET_CONN_BACKLOG;
    }
    
    if (!iSocketServer->ServerSocketOpen(Port, Backlog))
    {
      LogError( iLtpLogId, "ServerSocketOpen");
      return;
    }
  
    if (ESocketType_BT == iSocketType)
    {
      LogInfo( iLtpLogId, "ServerSocket is now Opened at Port(dynamic) and can acceptting MAX("<<Backlog<<") connections ...");
    }
    else
    {
      LogInfo( iLtpLogId, "ServerSocket is now Opened at Port "<<Port<<" and can acceptting MAX("<<Backlog<<") connections ...");
    }
  }
  else if (instance == std::string("SP: "))
  {
    // Create a SocketClient instance.
    if (ESocketType_BT == iSocketType)
    {
      LogInfo( iLtpLogId, "Create BluetoothSocketClient...");
      iBTSocketClient = new CLtpBluetoothSocketClient(instance);
      iSocketClient = iBTSocketClient;
    }
    else
    {
      LogInfo( iLtpLogId, "Create IPSocketClient...");
      iIpSocketClient = new CLtpIpSocketClient(instance);
      iSocketClient = iIpSocketClient;
    }

    // Open a connection socket.
    if (!iSocketClient->ClientSocketOpen())
    {
      LogInfo( iLtpLogId, "ClientSocketOpen");
      //exit(EXIT_FAILURE);
      return;
    }

    // Connecting to Addr with Port.
    const char* Addr;
    unsigned short Port;
    if (ESocketType_BT == iSocketType)
    {
      Addr = BT_SOCKET_SERVER_ADDR;
      Port = 0; //Input 0 to start dynamic port assignment returned by search_service().
    }
    else
    {
      Addr = SOCKET_SERVER_ADDR;
      Port = SOCKET_PORT;
    }
    
    if (!iSocketClient->ClientSocketConnect(Addr, Port))
    {
      LogInfo( iLtpLogId, "ClientSocketConnect");
      //exit(EXIT_FAILURE);
      return;
    }

    if (ESocketType_BT == iSocketType)
    {
      LogInfo( iLtpLogId, "ClientSocket is now Opened and Connectted to Addr("<<Addr<<") at Port(dynamic) ...");
    }
    else
    {
      LogInfo( iLtpLogId, "ClientSocket is now Opened and Connectted to Addr("<<Addr<<") at Port("<<Port<<") ...");
    }
  }
  else
  {
    LogInfo( iLtpLogId, "Socket connection create failed..............");
    //exit(EXIT_FAILURE);
    return;
  }
}

CLtpConnection::~CLtpConnection()
{
  //printf("~CLtpConnection this:0x%x", (unsigned long)this);
  iSpListener = NULL;
  iHuListener = NULL;
  sgHuListener = NULL;
  
  // Close the sockets.
  if (iBTSocketServer)
  {
    if (!iBTSocketServer->ServerSocketClose())
    {
      LogInfo( iLtpLogId, "Err ServerSocketClose");
    }
    
    delete iBTSocketServer;
  }

  if (iBTSocketClient)
  {
    if (!iBTSocketClient->ClientSocketClose())
    {
      LogInfo( iLtpLogId, "Err ClientSocketClose");
    }
    
    delete iBTSocketClient;
  }

  if (iIpSocketServer)
  {
    if (!iIpSocketServer->ServerSocketClose())
    {
      LogInfo( iLtpLogId, "Err ServerSocketClose");
    }
    
    delete iIpSocketServer;
  }

  if (iIpSocketClient)
  {
    if (!iIpSocketClient->ClientSocketClose())
    {
      LogInfo( iLtpLogId, "Err ClientSocketClose");
    }
    
    delete iIpSocketClient;
  }

  iSocketServer = NULL;
  iSocketClient = NULL;
  iBTSocketServer = NULL;
  iBTSocketClient = NULL;
  iIpSocketServer = NULL;
  iIpSocketClient = NULL;

}

#ifdef SOCKET_TEST
int CLtpConnection::socketTest(std::string instance)
{
  std::cout << "=============socketTestStart==============" << std::endl;
  printf("I am [%s]", instance.c_str());
  if (instance == std::string("HU: "))
  {
    // Create a SocketServer instance.
    if (ESocketType_BT == iSocketType)
    {
      printf("Create BluetoothSocketServer...");
      iBTSocketServer = new CLtpBluetoothSocketServer(instance);
      iSocketServer = iBTSocketServer;
    }
    else
    {
      printf("Create IPSocketServer...");
      iIpSocketServer = new CLtpIpSocketServer(instance);
      iSocketServer = iIpSocketServer;
    }
  
    // Open a listenning socket.
    unsigned short Port;
    int Backlog;
    if (ESocketType_BT == iSocketType)
    {
      Port = 0; //Input 0 to start dynamic port assignment.
      Backlog = BT_SOCKET_CONN_BACKLOG;
    }
    else
    {
      Port = SOCKET_PORT;
      Backlog = SOCKET_CONN_BACKLOG;
    }

    if (!iSocketServer->ServerSocketOpen(Port, Backlog))
    {
      perror("ServerSocketOpen");
      //exit(EXIT_FAILURE);
      return -1;
    }
  
    if (ESocketType_BT == iSocketType)
      printf("ServerSocket is now Opened at Port(dynamic) and can acceptting MAX(%d) connections ...", Backlog);
    else
      printf("ServerSocket is now Opened at Port(%d) and can acceptting MAX(%d) connections ...", Port, Backlog);
  }
  else if (instance == std::string("SP: "))
  {
    // Create a SocketClient instance.
    if (ESocketType_BT == iSocketType)
    {
      printf("Create BluetoothSocketClient...");
      iBTSocketClient = new CLtpBluetoothSocketClient(instance);
      iSocketClient = iBTSocketClient;
    }
    else
    {
      printf("Create IPSocketClient...");
      iIpSocketClient = new CLtpIpSocketClient(instance);
      iSocketClient = iIpSocketClient;
    }

    // Open a connection socket.
    if (!iSocketClient->ClientSocketOpen())
    {
      perror("ClientSocketOpen");
      //exit(EXIT_FAILURE);
      return -1;
    }

    // Connecting to Addr with Port.
    const char* Addr;
    unsigned short Port;
    if (ESocketType_BT == iSocketType)
    {
      Addr = BT_SOCKET_SERVER_ADDR;
      Port = 0; //Input 0 to start dynamic port assignment returned by search_service().
    }
    else
    {
      Addr = SOCKET_SERVER_ADDR;
      Port = SOCKET_PORT;
    }

    if (!iSocketClient->ClientSocketConnect(Addr, Port))
    {
      perror("ClientSocketConnect");
      //exit(EXIT_FAILURE);
      return -1;
    }

    if (ESocketType_BT == iSocketType)
      printf("ClientSocket is now Opened and Connectted to Addr(%s) at Port(dynamic) ...", Addr);
    else
      printf("ClientSocket is now Opened and Connectted to Addr(%s) at Port(%d) ...", Addr, Port);
  }
  else
  {
    std::cout << iInstance + "Socket connection create failed.............."<<std::endl;
    //exit(EXIT_FAILURE);
    return -1;
  }

  if (iSocketServer)
  {
    if (ESocketType_BT == iSocketType)
      iBTSocketServer->test(0, NULL);
    else
      iIpSocketServer->test(0, NULL);
  }
  else if (iSocketClient)
  {
    const char* linstNB[2];
    if (ESocketType_BT == iSocketType)
    {
      linstNB[0] = "BTSocketClient";
      linstNB[1] = "1C:4B:D6:0F:F1:56";
      iBTSocketClient->test(2, linstNB);
    }
    else
    {
      linstNB[0] = "IpSocketClient";
      linstNB[1] = SOCKET_SERVER_ADDR;
      iIpSocketClient->test(2, linstNB);
    }
  }
  else
  {
    if (ESocketType_BT == iSocketType)
      std::cout << "No BTSocket available, bypass testing." << std::endl;
    else
      std::cout << "No IpSocket available, bypass testing." << std::endl;
  }
  std::cout << "=============socketTestEnd==============" << std::endl;

  while (1);
}
#endif //SOCKET_TEST

void CLtpConnection::blockSocketSendMessage(const std::string& source, const std::string aMessage)
{
  if (source == std::string("HU: ")) 
  {
    // "HU" WRITE messages to SP's socket client, then exit after successfully sent message.
    // For SocketServer to send RESP or RESP_ERROR messages.
    if (!iSocketServer->ServerSocketWrite(aMessage.c_str()))
    {
      LogInfo( iLtpLogId, "ServerSocketWrite");
      //exit(EXIT_FAILURE);
      return;
    }
  } 
  else if (source == std::string("SP: ")) 
  {
    // "SP" WRITE messages to HU's socket server, then exit after successfully sent message.
    // For SocketClient to send REQ messages.
    if (!iSocketClient->ClientSocketWrite(aMessage.c_str()))
    {
      LogInfo( iLtpLogId, "ClientSocketWrite");
      //exit(EXIT_FAILURE);
      return;
    }

    // FIX ME:
    //       should have a thread for just receiving responses from HU.
    // SP should wait for the RESP or RESP_ERROR from HU.
    //if (!blockSocketReceiveMessage())
    //{
    //  LogInfo( iLtpLogId, "Err blockSocketReceiveMessage");
    //  return;
    //}
  } 
  else 
  {
    //throw new IllegalArgumentException("Unknown source: " + source);
    throw CLtpIllegalArgumentException("Unknown source: " + source);
  }
}

int CLtpConnection::blockSocketReceiveMessage()
{
  if (iInstance == std::string("HU: ")) 
  {
    // "HU" READ messages from SP's rfcomm socket client, then call the messageReceived() to notify RpcOverSpp layer listener that a message was received.
    // Note:
    // 1. The recieved message should be copied to a std::string buffer iMessageBuffer, 
    //    this buffer will be allocated and destroied along with CLtpConnection object.
    // 2. iMessageBuffer will be sent to RpcOverSpp via messageReceived().
    //------------------------------------------------------------------------------------
    // For SocketServer to receive one REQ, 
    char* aBuffer = NULL;
    if (!iSocketServer->ServerSocketRead(&aBuffer))
    {
      //Fix Me : maybe just close this connection and exit this thread.
      LogInfo( iLtpLogId, "Err ServerSocketRead");
      //exit(EXIT_FAILURE); 
      return 0;
    }
    iMessageBuffer = aBuffer;
    //------------------------------------------------------------------------------------

    // Create a thread to add this new message to pool.
    //------------------------------------------
    // Copy the message to heap.
    iMsgBuffer = new char[KMessageBufUnitSize];
    memset(iMsgBuffer, '\0', KMessageBufUnitSize);
    strncpy(iMsgBuffer, iMessageBuffer.c_str(), KMessageBufUnitSize);

    // Create a thread for adding this message(in iMsgBuffer) to the message pool.
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, &CLtpConnection::addOneMessageToPool, iMsgBuffer) < 0)
    {
      LogInfo( iLtpLogId, "Err pthread_create");
      return 0;
    }
    //------------------------------------------
    
    // DBG.
    //CLtpTimer::GoCountDownSleeper(2);
  } 
  else if (iInstance == std::string("SP: ")) 
  {
    // "SP" READ messages from HU's rfcomm socket server, then call the messageReceived() to notify RpcOverSpp layer listener that a message was received.
    // Note:
    // 1. The recieved message should be copied to a std::string buffer iMessageBuffer, 
    //    this buffer will be allocated and destroied along with CLtpConnection object.
    // 2. iMessageBuffer will be sent to RpcOverSpp via messageReceived().
    //------------------------------------------------------------------------------------
    // For SocketClient to receive the RESP or RESP_ERROR, 
    // the Connect() to socket server shall already established, so just R/W messages directly.
    char* aBuffer = NULL;
    if (!iSocketClient->ClientSocketRead(&aBuffer))
    {
      //Fix Me : maybe just close this connection and exit this thread.
      LogInfo( iLtpLogId, "ClientSocketRead");
      //exit(EXIT_FAILURE);
      return 0;
    }
    iMessageBuffer = aBuffer;
    //------------------------------------------------------------------------------------

    // Notify RpcOverSpp layer listener that a message was received
    if (iSpListener != NULL)
    {
      iSpListener->messageReceived(iMessageBuffer); 
    }
  } 
  else 
  {
    throw CLtpIllegalArgumentException("Unknown source: " + iInstance);
    return 0;
  }

  return 1;
}

bool CLtpConnection::startConnectorSocketServerLoop(unsigned int aMaxLoopCount)
{
  LogInterface( iLtpLogId, " aMaxLoopCount="<<aMaxLoopCount);
  
  bool isInfinity;
  isInfinity = (0 == aMaxLoopCount)? 1 : 0;

  // Create a thread to get all message from poll and handle them.
  //---------------------------------------
  // Create a thread for adding this message(in iMsgBuffer) to the message pool.
  pthread_t thread_id;
  if (pthread_create(&thread_id, NULL, &CLtpConnection::MessagePoolWorker, NULL) < 0)
  {
    LogInfo( iLtpLogId, "Err pthread_create");
    return false;
  }      
  //---------------------------------------
      
  while (1)
  {
    // The Accept() -> Read()/Write() procedure should be call while R/W messages.
    LogInfo( iLtpLogId, "*** Next ServerSocketAccept() for Read()/Write()...");
    if (!iSocketServer->ServerSocketAccept())
    {
      LogInfo( iLtpLogId, "Err: ServerSocketAccept not success!");
      //exit(EXIT_FAILURE);
      return false;
    }

    // Keep reading messags from client socket.
    while (1)
    {
      // HU should wait for the REQ from SP.
      if (!blockSocketReceiveMessage())
      {
        LogInfo( iLtpLogId, "Err blockSocketReceiveMessage");
        
        // We should break inner loop for next accept.
        break; 
      }

      //isInfinity = 1;
      if (!isInfinity)
      {
        if (0 == --aMaxLoopCount)
          return false;
      }
    }
  }
}

void CLtpConnection::registerListener(const std::string& source, ILtpConnectionListener& listener)
{
  if (source == std::string("HU: ")) 
  {
    iHuListener = &listener;
    sgHuListener = &listener;
    //printf("registerListener sgHuListener: %d", (int)sgHuListener);
  } 
  else if (source == std::string("SP: ")) 
  {
    iSpListener = &listener;
  } 
  else 
  {
    throw CLtpIllegalArgumentException("Unknown source: " + source);
  }
}

void CLtpConnection::unregisterListener(const std::string& source)
{
  if (source == std::string("HU: ")) 
  {
    iHuListener = NULL;
    sgHuListener = NULL;
  } 
  else if (source == std::string("SP: ")) 
  {
    iSpListener = NULL;
  } 
  else 
  {
    throw CLtpIllegalArgumentException("Unknown source: " + source);
  }
}

void* CLtpConnection::addOneMessageToPool(void* aMsg)
{
  assert(NULL != aMsg);
  
  std::string Msg = (char*)aMsg;

  // Delete the message buffer.
  delete [](char*)aMsg;
  
  // Add this message to pool.
  //---------------------------------------
  if (sgMutexMsgPool.lock() != 0)
  {
    LogInfo( iLtpLogId, "pthread_mutex_lock");
    //exit(EXIT_FAILURE);
    pthread_exit(NULL); //give up this thread.
  }

  LogInfo( iLtpLogId, "Add: ["<<sgMsgSerialNum<<", "<<Msg.c_str()<<"]");
  sgMsgPool.insert(std::pair<int, std::string>(sgMsgSerialNum, Msg));
  sgMsgSerialNum++;

  if (sgMutexMsgPool.unlock() != 0)
  {
    LogInfo( iLtpLogId, "pthread_mutex_unlock");
    //exit(EXIT_FAILURE);
    pthread_exit(NULL); //give up this thread.
  }

  // Wake sleeping thread CLtpConnection::MessagePoolWorker
  if (sgCondVarMsgPool.signal() != 0)
  {
    LogInfo( iLtpLogId, "pthread_cond_signal");
    //exit(EXIT_FAILURE);
    pthread_exit(NULL); //give up this thread.
  }
  //---------------------------------------

  pthread_exit(NULL);
}

void* CLtpConnection::MessagePoolWorker(void* unuse)
{
  std::string Msg;
  LogInterface( iLtpLogId, "");

  Msg.clear();
  while (1)
  {
    // Get one message from pool and remove it, then send to protocol layers for decoding.
    //---------------------------------------
    if (sgMutexMsgPool.lock() != 0)
    {
      LogInfo( iLtpLogId, "pthread_mutex_lock");
      //exit(EXIT_FAILURE);
      pthread_exit(NULL); //give up this thread.
    }

    // Check that shared variable is not in state we want
    while (sgMsgPool.size() == 0)
    {
      LogInfo( iLtpLogId, "Thread CLtpConnection::MessagePoolWorker wait for sgCondVarMsgPool...");
      if (sgCondVarMsgPool.wait() != 0)
      {
        LogInfo( iLtpLogId, "pthread_cond_wait");
        //exit(EXIT_FAILURE);
        pthread_exit(NULL); //give up this thread.
      }
    }

    // DBG.
    //CLtpTimer::GoCountDownSleeper(2);

    // DBG.
    LogInfo( iLtpLogId, "sgMsgPool.size():"<<sgMsgPool.size());

    // Now shared variable is in desired state; do the desired work.
    // Loop through the sgMsgPool and handle each request.
    while (sgMsgPool.size() > 0)//if (sgMsgPool.size() > 0)
    {
      // Get and Remove first message.
      std::map<int, std::string>::iterator it = sgMsgPool.begin();
      Msg = it->second;
      sgMsgPool.erase(it);

      // Notify RpcOverSpp layer listener that a message was received
      if (sgHuListener != NULL)
      {
        if (Msg.length() != 0)
        {
          sgHuListener->messageReceived(Msg);
          Msg.clear();
        }
      }
    }

    if (sgMutexMsgPool.unlock() != 0)
    {
      LogInfo( iLtpLogId, "pthread_mutex_unlock");
      //exit(EXIT_FAILURE);
      pthread_exit(NULL); //give up this thread.
    }
    //---------------------------------------
  }
  
  pthread_exit(NULL);
}

