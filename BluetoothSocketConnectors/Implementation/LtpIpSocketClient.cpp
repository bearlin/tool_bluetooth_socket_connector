//
// This file contain CLtpIpSocketClient implementation for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpIpSocketClient.h"
#include "ILogging.h"

using namespace NBsc;

extern logging::TLogComponent iLtpLogId;

static struct TReadOneLineBuf sgRlBufRead;

CLtpIpSocketClient::CLtpIpSocketClient(std::string instance) : 
  iInstance(instance), 
  iFdSocketConnect(0),
  iBuffer(NULL)
{
  iBuffer = new char[KMessageBufUnitSize];
  //printf("Allocated iBuffer size = (%d) bytes\n", KMessageBufUnitSize);
  memset(iBuffer, '\0', KMessageBufUnitSize);
}

CLtpIpSocketClient::~CLtpIpSocketClient()
{
  if (iBuffer)
    delete []iBuffer;
}

int CLtpIpSocketClient::client_start(int argc, char **argv)
{
  LogInterface( iLtpLogId, "");

  int status = 0;

  // The socket server IPV4 address will be passed into this function via argv[].
  if(argc != 2)
  {
    LogInfo( iLtpLogId, "Usage: "<<argv[0]<<" <ip of server>");
    return -1;
  } 

  // Create an socket endpoint for communication
  int fd_socket = 0;//, n = 0;
  fd_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (fd_socket < 0)
  {
    LogInfo( iLtpLogId, "Socket create failed");
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket create ok!\n");

  // Initiate a connection to server socket at port SOCKET_PORT
  struct sockaddr_in serv_addr; 
  memset(&serv_addr, '0', sizeof(serv_addr)); 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SOCKET_PORT); 
  status = inet_pton(AF_INET, argv[1], &serv_addr.sin_addr); //convert IPv4 and IPv6 addresses from text to binary form 
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket inet_pton failed");
    close(fd_socket);
    //exit(EXIT_FAILURE);
    return -1;
  }
  status = connect(fd_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
  if (status < 0)
  {
    LogInfo( iLtpLogId, "Socket connect failed");
    close(fd_socket);
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket connect ok!");

  char Buffer[1024];
  memset(Buffer, 0,sizeof(Buffer));
#ifdef SEND_TO_CLIENT
  int length = 0;
  // Read from socket file descriptor
  length = read(fd_socket, Buffer, sizeof(Buffer)-1);
  if(length < 0)
  {
    LogInfo( iLtpLogId, "Socket read error");
    close(fd_socket);
    //exit(EXIT_FAILURE);
    return -1;
  } 
  Buffer[length] = 0;
  LogInfo( iLtpLogId, "Received: "<<Buffer);

#else //SEND_TO_CLIENT
  // Write to socket file descriptor
  time_t ticks; 
  ticks = time(NULL);
  snprintf(Buffer, sizeof(Buffer), "%.24s\r\n", ctime(&ticks));
  
  status = write(fd_socket, Buffer, strlen(Buffer)); 
  if (status < 0)
  {
    LogInfo( iLtpLogId, "Socket write error");
    close(fd_socket);
    //exit(EXIT_FAILURE);
    return -1;
  } 
  else
  {
    LogInfo( iLtpLogId, "Write "<<status<<" bytes done\n");
  }

#endif //SEND_TO_CLIENT

  // Close this socket connection.
  status = close(fd_socket);
  if (status < 0)
  {
    LogInfo( iLtpLogId, "close socket error");
    //exit(EXIT_FAILURE);
    return -1;
  }

  return 0;
}

int CLtpIpSocketClient::ClientSocketOpen()
{
  // Create an socket endpoint for communication
  iFdSocketConnect = socket(AF_INET, SOCK_STREAM, 0);
  if(iFdSocketConnect < 0)
  {
    LogInfo( iLtpLogId, "Socket create failed");
    return 0;
  }
  
  return 1;
}

int CLtpIpSocketClient::ClientSocketConnect(const char* aAddr, unsigned short aPort)
{
  int status = 0;
  
  // Initiate a connection to server socket at port SOCKET_PORT
  struct sockaddr_in serv_addr; 
  memset(&serv_addr, 0, sizeof(serv_addr)); 
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(aPort); 
  status = inet_pton(AF_INET, aAddr, &serv_addr.sin_addr); //convert IPv4 and IPv6 addresses from text to binary form 
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket inet_pton failed");
    return 0;//exit(EXIT_FAILURE);
  }
  status = connect(iFdSocketConnect, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
  if (status < 0)
  {
    LogInfo( iLtpLogId, "ClientSocketConnect failed!");
    return 0;
  }

  CLtpCommonTools::readOneLineBufInit(iFdSocketConnect, &sgRlBufRead);

  LogInfo( iLtpLogId, "Socket connect to Addr("<<aAddr<<") at Port("<<aPort<<") ok!\n");
  
  return 1;
}

int CLtpIpSocketClient::ClientSocketRead(char** aBuffer)
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
  LogInfo( iLtpLogId, "Received ("<<length<<") bytes: "<< iBuffer);

  //CLtpCommonTools::printBufHex(iBuffer, 200/*KMessageBufUnitSize*/);

  *aBuffer = iBuffer;

  return 1;
}

int CLtpIpSocketClient::ClientSocketWrite(const char* aBuffer)
{
  LogInterface( iLtpLogId, "");
  int length = 0;
  int aBuffer_strlen = 0;
  
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
    //printf("aBuffer_strlen[%d]\n", aBuffer_strlen);
  
    if (KMessageBufUnitSize < (aBuffer_strlen + 2)) // reserve 1 byte for '\n'+'\0'.
    {
      LogInfo( iLtpLogId, "Character string too long! Current allocated buffer size = ("<<KMessageBufUnitSize<<") bytes.");
      return 0;//exit(EXIT_FAILURE);
    }

    // Copy data to iBuffer.
    strncpy(iBuffer, aBuffer, aBuffer_strlen);

    // Make sure the string end with '\r' or '\n' for Android.
    if ( ('\r' != iBuffer[aBuffer_strlen-1]) && ('\n' != iBuffer[aBuffer_strlen-1]) )
    {
      iBuffer[aBuffer_strlen] = '\n';
      //printf("Force iBuffer[%d] with '\\n'\n", aBuffer_strlen);
    }

    //null character manually added at string end
    iBuffer[(aBuffer_strlen+1)] = '\0'; 
    //printf("Force iBuffer[%d] with '\\0'\n", (aBuffer_strlen+1));
  }

  //printf("iBuffer before send:%s\n", iBuffer);
  //printf("strlen(iBuffer):%d\n", strlen(iBuffer));
  //CLtpCommonTools::printBufHex(iBuffer, 200/*KMessageBufUnitSize*/);

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
  LogInfo( iLtpLogId, "Write ("<<length<<") bytes:"<<iBuffer);

  return 1;
}

int CLtpIpSocketClient::ClientSocketClose()
{
  int status = 0;
  
  // Close this socket connection.
  status = close(iFdSocketConnect);
  if (status < 0)
  {
    LogInfo( iLtpLogId, "close connection socket error");
    return 0;//exit(EXIT_FAILURE);
  }

  return 1;
}

int CLtpIpSocketClient::test(int argc, const char **argv)
{
#if 0
  return client_start(argc, argv);
#else
  #if 0
  // Open a connection socket.
  if (!ClientSocketOpen())
    printf("Err ClientSocketOpen\n");
  if (!ClientSocketConnect(SOCKET_SERVER_ADDR, SOCKET_PORT))
    printf("Err ClientSocketConnect\n");
  #endif

  // Test Write -> loop read.
  if (!ClientSocketWrite(NULL))
    printf("Err ClientSocketWrite\n");

  while (1)
  {
    char* aBuffer = NULL;
    if (!ClientSocketRead(&aBuffer))
      printf("Err ClientSocketRead\n");
  }

  #if 0
  // Close the socket.
  if (!ClientSocketClose())
    printf("Err ClientSocketClose\n");
  #endif

  return 1;
#endif

}


