//
// This file contain CLtpIpSocketServer implementation for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpIpSocketServer.h"
#include "ILogging.h"

using namespace NBsc;

extern logging::TLogComponent iLtpLogId;

static struct TReadOneLineBuf sgRlBufRead;

CLtpIpSocketServer::CLtpIpSocketServer(std::string instance) : 
  iInstance(instance),
  iFdSocketListen(0), 
  iFdSocketConnect(0),
  iBuffer(NULL)
{
  iBuffer = new char[KMessageBufUnitSize];
  //printf("Allocated iBuffer size = (%d) bytes\n", KMessageBufUnitSize);
  memset(iBuffer, '\0', KMessageBufUnitSize);
}

CLtpIpSocketServer::~CLtpIpSocketServer()
{
  if (iBuffer)
    delete []iBuffer;
}

int CLtpIpSocketServer::server_start(int argc, char **argv)
{
  LogInterface( iLtpLogId, "");

  int status = 0;

  // Create an socket endpoint for communication
  int fd_socket_listen = 0;
  fd_socket_listen = socket(AF_INET, SOCK_STREAM, 0);
  if(fd_socket_listen < 0)
  {
    LogInfo( iLtpLogId, "Socket create failed");
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket create ok!");

  // Bind a name to a socket at port SOCKET_PORT
  struct sockaddr_in serv_addr; 
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(SOCKET_PORT); 
  status = bind(fd_socket_listen, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket bind failed");
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket bind to port: " << SOCKET_PORT);

  // Listen for connections on th socket fd_socket_listen.
  // SOCKET_CONN_BACKLOG = The maximum length to which the queue of pending connections.
  status = listen(fd_socket_listen, SOCKET_CONN_BACKLOG); 
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket listen failed");
    close(fd_socket_listen);
    //exit(EXIT_FAILURE);
    return -1;
  } 
  LogInfo( iLtpLogId, "Socket listening...");

  // Loop for accept() client -> R/W client -> close client.
  char Buffer[1024L];
  int sin_size; 
  int fd_socket_connect = 0;
  struct sockaddr_in client_addr; 
  memset(Buffer, 0, sizeof(Buffer)); 
  while (1)
  {
    // Accept a connection on a socket.
    // It extracts the first connection request on the queue of pending connections for the listening socket,
    // fd_socket_listen, creates a new connected socket, and returns a new file descriptor referring to that socket.  
    // The newly created socket is not in the listening state.  
    // The original socket sockfd is unaffected by this call.
    LogInfo( iLtpLogId, "ready to accept...");
    //fd_socket_connect = accept(fd_socket_listen, (struct sockaddr*)NULL, NULL); 
    sin_size = sizeof(struct sockaddr_in); 
    fd_socket_connect = accept(fd_socket_listen, (struct sockaddr*)&client_addr, (socklen_t*)&sin_size); 
    if(fd_socket_connect < 0)
    {
      LogInfo( iLtpLogId, "Socket client accept error");
      close(fd_socket_listen);
      //exit(EXIT_FAILURE);
      return -1;
    }

    // Retrieve the address string from acceptted client_addr.
    //http://blog.sina.com.cn/s/blog_6151984a0100etj1.html
    char* addr_str;
    addr_str = inet_ntoa(client_addr.sin_addr);
    LogInfo( iLtpLogId, "Accept connection from addr: "<<addr_str); 

  #ifdef SEND_TO_CLIENT
    // Write to socket file descriptor
    time_t ticks; 
    ticks = time(NULL);
    snprintf(Buffer, sizeof(Buffer), "%.24s\r\n", ctime(&ticks));
    write(fd_socket_connect, Buffer, strlen(Buffer)); 
  #else //SEND_TO_CLIENT
    // Read from socket file descriptor
    int length = 0;
    length = read(fd_socket_connect, Buffer, sizeof(Buffer)-1);
    if (length < 0)
    {
      LogInfo( iLtpLogId, "Socket read error");
      close(fd_socket_connect);
      close(fd_socket_listen);
      //exit(EXIT_FAILURE);
      return -1;
    } 
  Buffer[length] = 0;
  LogInfo( iLtpLogId, "Received: "<< Buffer);
  #endif //SEND_TO_CLIENT

    // Close this acceptted client connection.
    status = close(fd_socket_connect);
    if (status < 0)
    {
      LogInfo( iLtpLogId, "Close client socket error");
      close(fd_socket_listen);
      //exit(EXIT_FAILURE);
      return -1;
    }
    //sleep(1);
  }

  // Close listenning server connection.
  status = close(fd_socket_listen);
  if (status < 0)
  {
    LogInfo( iLtpLogId, "close client socket error");
    //exit(EXIT_FAILURE);
    return -1;
  }
  
  return 0;
}

int CLtpIpSocketServer::ServerSocketOpen(unsigned short aPort, int aBacklog)
{
  int status = 0;

  // Create an socket endpoint for communication
  iFdSocketListen = socket(AF_INET, SOCK_STREAM, 0);
  if (iFdSocketListen < 0)
  {
    LogInfo( iLtpLogId, "Socket create failed");
    return 0;//exit(EXIT_FAILURE);
  }
  //printf("Socket create ok!\n");
  
  // Enable address reuse.
  int on = 1;
  status = setsockopt(iFdSocketListen, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket setsockopt() failed");
    return 0;//exit(EXIT_FAILURE);
  }
  LogInfo( iLtpLogId, "Socket enable address reuse OK!");
  
  // Bind a name to a socket at port aPort
  struct sockaddr_in serv_addr; 
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(aPort); 
  status = bind(iFdSocketListen, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket bind failed");
    return 0;//exit(EXIT_FAILURE);
  }
  //printf("Socket bind to port(%d)!\n", aPort);

  // Listen for connections on th socket iFdSocketListen.
  status = listen(iFdSocketListen, aBacklog); 
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket listen failed");
    close(iFdSocketListen);
    return 0;//exit(EXIT_FAILURE);
  } 
  //printf("Socket listening with Backlog(%d)...!\n", aBacklog);

  return 1;
}

int CLtpIpSocketServer::ServerSocketAccept()
{

  int sin_size; 
  struct sockaddr_in client_addr; 

  // Accept a connection on a socket.
  // It extracts the first connection request on the queue of pending connections for the listening socket,
  // creates a new connected socket, and returns a new file descriptor referring to that socket.  
  // The newly created socket is not in the listening state.  
  // The original socket sockfd is unaffected by this call.
  // It blocks the caller until a connection is present. 
  LogInfo( iLtpLogId, "ready to accept...");
  //iFdSocketConnect = accept(iFdSocketListen, (struct sockaddr*)NULL, NULL); 
  sin_size = sizeof(struct sockaddr_in); 
  iFdSocketConnect = accept(iFdSocketListen, (struct sockaddr*)&client_addr, (socklen_t*)&sin_size); 
  if(iFdSocketConnect < 0)
  {
    LogInfo( iLtpLogId, "Socket client accept error");
    //close(iFdSocketListen);
    return 0;//exit(EXIT_FAILURE);
  }

  CLtpCommonTools::readOneLineBufInit(iFdSocketConnect, &sgRlBufRead);

  // Retrieve the address string from acceptted client_addr.
  //http://blog.sina.com.cn/s/blog_6151984a0100etj1.html
  char* addr_str;
  addr_str = inet_ntoa(client_addr.sin_addr);
  LogInfo( iLtpLogId, "Accept connection from addr: "<<addr_str); 
  
  return 1;
}

int CLtpIpSocketServer::ServerSocketRead(char** aBuffer)
{

  int length = 0;

  // Read from socket file descriptor
  memset(iBuffer, '\0', KMessageBufUnitSize); //clear buffer first.

  // FIX ME:
  // We should use the timeout feature of the select() or poll() system calls to prevent long blocking.
  //
  //length = read(iFdSocketConnect, iBuffer, KMessageBufUnitSize-1);  //On success, the number of bytes read is returned (zero indicates end of file)
  //length = recv(iFdSocketConnect, iBuffer, KMessageBufUnitSize-1, 0); //On success, the number of bytes read is returned (zero indicates end of file)
  //length = CLtpCommonTools::readLine(iFdSocketConnect, iBuffer, KMessageBufUnitSize-1);
  length = CLtpCommonTools::readOneLineBuf(&sgRlBufRead, iBuffer, KMessageBufUnitSize-1);
  if (length <= 0)
  {
    LogInfo( iLtpLogId, "Socket read error or connection closed!");
    //close(iFdSocketConnect);
    return 0;//exit(EXIT_FAILURE);
  }
  iBuffer[KMessageBufUnitSize-1] = '\0'; /* null character manually added at buffer end */
  iBuffer[length] = '\0';
  LogInfo( iLtpLogId, "Received ("<<length<<") bytes:"<<iBuffer);

  //CLtpCommonTools::printBufHex(iBuffer, 200/*KMessageBufUnitSize*/);

  *aBuffer = iBuffer;
  //printf("iBuffer(0x%x), *aBuffer(0x%x)\n", iBuffer, *aBuffer);

  return 1;
}

int CLtpIpSocketServer::ServerSocketWrite(const char* aBuffer)
{

  int length = 0;
  int aBuffer_strlen = 0;
  
  //printf("KMessageBufUnitSize[%d]\n", KMessageBufUnitSize);
  
  // Write to socket file descriptor
  //
  // #####################
  // NOTE:
  // We need to force the iBuffer string ending with ('\r'+'\n'+'\0') or ('\r'+''\0') or ('\n'+'\0') to let Android Bluetooth Socket receive string correctly!
  // If the string doesn't end with '\r' or '\n', Android won't treat it as receive done.
  // #####################
  memset(iBuffer, '\0', KMessageBufUnitSize); //clear buffer first.
  if (NULL == aBuffer)
  {
    // Create timestamp string to iBuffer.
    time_t ticks; 
    ticks = time(NULL);
    snprintf(iBuffer, KMessageBufUnitSize, "%.24s\r\n", ctime(&ticks)); //string ending with ('\r'+'\n'+'\0')
  }
  else
  {
    aBuffer_strlen = strlen(aBuffer);
  
    if (KMessageBufUnitSize < (aBuffer_strlen + 2)) // reserve 1 byte for '\n'+'\0'.
    {
      LogInfo( iLtpLogId, "Character string too long! Current allocated buffer size = "<<KMessageBufUnitSize);
      return 0;//exit(EXIT_FAILURE);
    }

    // Copy data to iBuffer.
    strncpy(iBuffer, aBuffer, aBuffer_strlen);

    // Make sure the string end with '\r' or '\n' for Android.
    if ( ('\r' != iBuffer[aBuffer_strlen-1]) && ('\n' != iBuffer[aBuffer_strlen-1]) )
    {
      iBuffer[aBuffer_strlen] = '\n';
    }

    //null character manually added at string end
    iBuffer[(aBuffer_strlen+1)] = '\0';
  }

  // FIX ME:
  // We should use the timeout feature of the select() or poll() system calls to prevent long blocking.
  //
  //length = write(iFdSocketConnect, iBuffer, strlen(iBuffer)); //On success, the number of bytes written is returned (zero indicates nothing was written).
  //length = CLtpCommonTools::write_all(iFdSocketConnect, iBuffer, strlen(iBuffer));
  length = send(iFdSocketConnect, iBuffer, strlen(iBuffer), 0); //On success, the number of bytes written is returned (zero indicates nothing was written).
  if (length <= 0)
  {
    LogInfo( iLtpLogId, "Socket write error");
    //close(iFdSocketConnect);
    return 0;//exit(EXIT_FAILURE);
  } 
  LogInfo( iLtpLogId, "Write ("<<length<<") bytes: "<< iBuffer);

  return 1;
}

int CLtpIpSocketServer::ServerSocketClose()
{
  int status = 0;
  
  // Close this socket connection.
  status = close(iFdSocketConnect);
  if (status < 0)
  {
    LogInfo( iLtpLogId, "close connection socket error");
    return 0;//exit(EXIT_FAILURE);
  }
  
  // Close listenning server connection.
  status = close(iFdSocketListen);
  if (status < 0)
  {
    LogInfo( iLtpLogId, "close listenning socket error");
    return 0;//exit(EXIT_FAILURE);
  }

  return 1;
}

int CLtpIpSocketServer::test(int argc, char **argv)
{
#if 0
  return server_start(argc, argv);
#else
  #if 0
  // Open a listenning socket and start acceptting.
  if (!ServerSocketOpen(SOCKET_PORT, SOCKET_CONN_BACKLOG))
    printf("Err ServerSocketOpen\n");
  #endif

  //  Start acceptting.
  if (!ServerSocketAccept())
    printf("Err ServerSocketAccept\n");
  
  // Test Read -> loop write.
  char* aBuffer = NULL;
  if (!ServerSocketRead(&aBuffer))
    printf("Err ServerSocketRead\n");

  while (1)
  {
    printf("sleep(2)...\n");
    sleep(2);
    if (!ServerSocketWrite("dddddddddddddddddd"))//"dddddddddddddddddd\r\n" //"dddddddddddddddddd"
      printf("Err ServerSocketWrite\n");
  }

  #if 0
  // Close the socket.
  if (!ServerSocketClose())
    printf("Err ServerSocketClose\n");
  #endif

  return 1;
#endif

}


