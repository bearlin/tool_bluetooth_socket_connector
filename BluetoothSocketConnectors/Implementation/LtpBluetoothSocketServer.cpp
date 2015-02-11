//
// This file contain CLtpBluetoothSocketServer implementation for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpBluetoothSocketServer.h"
#include "ILogging.h"

#ifdef __cplusplus
  // *** BDADDR_ANY Constructs a temporary object and uses its address as below. This isn't allowed in C++.   ***
  // *** bluetooth.h:                                                                                         *** 
  // *** #define BDADDR_ANY   (&(bdaddr_t) {{0, 0, 0, 0, 0, 0}})                                              *** 
  // *** #define BDADDR_ALL   (&(bdaddr_t) {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}})                            *** 
  // *** #define BDADDR_LOCAL (&(bdaddr_t) {{0, 0, 0, 0xff, 0xff, 0xff}})                                     *** 
  // *** We don't want to modify bluetooth.h and don't want to add C++ compiler flag [-fpermissive] to fix it,***
  // *** so just replace BDADDR_ANY with my_BDADDR_ANY global variable.                                       ***
  // *** Ref: http://stackoverflow.com/questions/9751710/c-c-warning-address-of-temporary-with-bdaddr-any-bluetooth-library
  // ***      http://stackoverflow.com/questions/8843818/what-does-the-fpermissive-flag-do
  bdaddr_t my_BDADDR_ANY = {{0}};
  bdaddr_t my_BDADDR_LOCAL = {{0, 0, 0, 0xff, 0xff, 0xff}};
#endif //__cplusplus

using namespace NBsc;

extern logging::TLogComponent iLtpLogId;

static struct TReadOneLineBuf sgRlBufRead;

CLtpBluetoothSocketServer::CLtpBluetoothSocketServer(std::string instance) : 
  iInstance(instance),
  iFdSocketListen(0), 
  iFdSocketConnect(0),
  iBuffer(NULL),
  iSession(NULL)
{
  iBuffer = new char[KMessageBufUnitSize];
  memset(iBuffer, '\0', KMessageBufUnitSize);
}

CLtpBluetoothSocketServer::~CLtpBluetoothSocketServer()
{
  if (iBuffer)
    delete []iBuffer;
}

sdp_session_t* CLtpBluetoothSocketServer::register_service(int port)
{
  /*
  //fa87c0d0-afac-11de-8a39-0800200c9a66    // Linux <-> Linux.
  uint8_t svc_uuid_int[] = 
  { 
  0xfa, 0x87, 0xc0, 0xd0, 
  0xaf, 0xac, 0x11, 0xde, 
  0x8a, 0x39, 0x08, 0x00, 
  0x20, 0x0c, 0x9a, 0x66
  };
  */
  
  //00001101-0000-1000-8000-00805F9B34FB    // Linux <-> Linux/Android, use this one for Android phone.
  uint8_t svc_uuid_int[] = 
  { 
  0x00, 0x00, 0x11, 0x01, 
  0x00, 0x00, 0x10, 0x00, 
  0x80, 0x00, 0x00, 0x80, 
  0x5F, 0x9B, 0x34, 0xFB
  };
  
  /*
  //a3dd6d39-518c-4032-b226-f61d91052ca2    // For testing.
  uint8_t svc_uuid_int[] = 
  { 
  0xa3, 0xdd, 0x6d, 0x39, 
  0x51, 0x8c, 0x40, 0x32, 
  0xb2, 0x26, 0xf6, 0x1d, 
  0x91, 0x05, 0x2c, 0xa2 
  };
  */

  /*
  //00000000-0000-0000-0000-00000000abcd    // Linux <-> Linux testing.
  uint8_t svc_uuid_int[] = 
  { 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0xab, 0xcd 
  };
  */

  uint8_t rfcomm_channel = port;//11;

  const char *service_name = "BearLin_BTService";
  const char *svc_dsc = "Mobile_Connector";
  const char *service_prov = "TT";

  uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid, svc_class_uuid;

  sdp_list_t *l2cap_list = 0, 
             *rfcomm_list = 0,
             *root_list = 0,
             *proto_list = 0, 
             *access_proto_list = 0,
             *svc_class_list = 0,
             *profile_list = 0;
  sdp_data_t *channel = 0;
  sdp_profile_desc_t profile;
  sdp_record_t record = { 0 };
  sdp_session_t *session = 0;

  int status = 0;
  LogInfo( iLtpLogId, "------------ register_service start ------------");
  // Set the general service ID
  sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
  sdp_set_service_id( &record, svc_uuid );

  // Set the service class
  sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
  svc_class_list = sdp_list_append(0, &svc_class_uuid); //appen to linked list.
  sdp_set_service_classes(&record, svc_class_list);

  // Set the Bluetooth profile information
  sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
  profile.version = 0x0100;
  profile_list = sdp_list_append(0, &profile);
  sdp_set_profile_descs(&record, profile_list);

  // Make the service record publicly browsable
  sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
  root_list = sdp_list_append(0, &root_uuid);
  sdp_set_browse_groups( &record, root_list );

  // Set l2cap information
  sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
  l2cap_list = sdp_list_append( 0, &l2cap_uuid );
  proto_list = sdp_list_append( 0, l2cap_list );

  // Register the RFCOMM channel for RFCOMM sockets
  sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
  channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
  rfcomm_list = sdp_list_append( 0, &rfcomm_uuid );
  sdp_list_append( rfcomm_list, channel );
  sdp_list_append( proto_list, rfcomm_list );

  access_proto_list = sdp_list_append( 0, proto_list );
  sdp_set_access_protos( &record, access_proto_list );

  // Set the name, provider, and description
  sdp_set_info_attr(&record, service_name, service_prov, svc_dsc);

  // Connect to the local SDP server
#ifdef __cplusplus
  session = sdp_connect(&my_BDADDR_ANY, &my_BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
#else //__cplusplus
  session = sdp_connect(BDADDR_ANY, BDADDR_LOCAL, SDP_RETRY_IF_BUSY);
#endif //__cplusplus
  if (!session) 
  {
    // Cleanup and exit.
    LogInfo( iLtpLogId, "Failed to connect to the local SDP server errno="<<errno);
    sdp_data_free( channel );
    sdp_list_free( l2cap_list, 0 );
    sdp_list_free( rfcomm_list, 0 );
    sdp_list_free( root_list, 0 );
    sdp_list_free( access_proto_list, 0 );
    sdp_list_free( svc_class_list, 0 );
    sdp_list_free( profile_list, 0 );
    //exit(EXIT_FAILURE);
    return NULL;
  }
  LogInfo( iLtpLogId, "sdp_connect OK!");
  
  // Register the service record
  status = sdp_record_register(session, &record, 0);
  if (status < 0)
  {
    // Cleanup and exit.
    LogError( iLtpLogId, "sdp_record_register error");
    sdp_data_free( channel );
    sdp_list_free( l2cap_list, 0 );
    sdp_list_free( rfcomm_list, 0 );
    sdp_list_free( root_list, 0 );
    sdp_list_free( access_proto_list, 0 );
    sdp_list_free( svc_class_list, 0 );
    sdp_list_free( profile_list, 0 );
    //exit(EXIT_FAILURE);
    return NULL;
  }
  LogInfo( iLtpLogId, "sdp_record_register OK!");

  // Cleanup and return service session.
  sdp_data_free( channel );
  sdp_list_free( l2cap_list, 0 );
  sdp_list_free( rfcomm_list, 0 );
  sdp_list_free( root_list, 0 );
  sdp_list_free( access_proto_list, 0 );
  sdp_list_free( svc_class_list, 0 );
  sdp_list_free( profile_list, 0 );
  LogInfo( iLtpLogId, "------------ register_service done ------------");
  return session;
}

int CLtpBluetoothSocketServer::server_start(int argc, char **argv)
{
  LogInterface( iLtpLogId, "");

  int status = 0;

  // Create an socket endpoint for communication
  int fd_socket_listen = 0;
  fd_socket_listen = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if(fd_socket_listen < 0)
  {
    LogError( iLtpLogId, "Socket create failed");
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket create ok!");

  // Bind a name to a BT socket port
  struct sockaddr_rc serv_addr = { 0 };
  int port = 0;
  serv_addr.rc_family = AF_BLUETOOTH;
#ifdef __cplusplus
  serv_addr.rc_bdaddr = my_BDADDR_ANY;
#else //__cplusplus
  serv_addr.rc_bdaddr = *BDADDR_ANY;
#endif //__cplusplus
#ifndef BT_SOCKET_FIXED_PORT
  for (port = BT_SOCKET_MIN_PORT; port <= BT_SOCKET_MAX_PORT; port++)
  {
    serv_addr.rc_channel = port;
    status = bind(fd_socket_listen, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(status < 0)
    {
      LogError( iLtpLogId, "Socket bind failed");
    }
    if(status == 0)
      break;
  }
  if (port <= BT_SOCKET_MAX_PORT)
  {
    LogInfo( iLtpLogId, "Socket dynamic bind to port:"<<serv_addr.rc_channel);
  }
  else
  {
    LogError( iLtpLogId, "Dynamic bind port failed!");
    //exit(EXIT_FAILURE);
    return -1;
  }
#else //BT_SOCKET_FIXED_PORT
  port = BT_SOCKET_FIXED_PORT;
  serv_addr.rc_channel = port;
  status = bind(fd_socket_listen, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if(status < 0)
  {
    LogError( iLtpLogId, "bind error");
  }
  LogInfo( iLtpLogId, "bind to fixed port#"<<serv_addr.rc_channel);
#endif //BT_SOCKET_FIXED_PORT

  // Register the "TT_BTService" service.
  sdp_session_t* session = register_service(port);
  if (NULL == session)
  {
    LogError( iLtpLogId, "Advertising new services Failed ...");
    return -1;
  }
  LogInfo( iLtpLogId, "Advertising new services START ...");

  // Listen for connections on th socket fd_socket_listen.
  // SOCKET_CONN_BACKLOG = The maximum length to which the queue of pending connections.
  status = listen(fd_socket_listen, BT_SOCKET_CONN_BACKLOG); 
  if(status < 0)
  {
    LogError( iLtpLogId, "Socket listen failed");
    close(fd_socket_listen);
    sdp_close(session);
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket listening...");

  // Loop for accept() client -> R/W client -> close client.
  char Buffer[1024L];
  int sin_size; 
  int fd_socket_connect = 0;
  struct sockaddr_rc client_addr = { 0 };
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
    sin_size = sizeof(struct sockaddr_rc);
    fd_socket_connect = accept(fd_socket_listen, (struct sockaddr*)&client_addr, (socklen_t*)&sin_size); 
    if(fd_socket_connect < 0)
    {
      LogError( iLtpLogId, "Socket client accept error");
      close(fd_socket_listen);
      sdp_close(session);
      //exit(EXIT_FAILURE);
      return -1;
    }

    // Retrieve the address string from acceptted client_addr.
    ba2str( &client_addr.rc_bdaddr, Buffer );
    LogInfo( iLtpLogId, "Accept connection from addr:"<<Buffer);
    memset(Buffer, 0, sizeof(Buffer));

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
      LogError( iLtpLogId, "Socket read error");
      close(fd_socket_connect);
      close(fd_socket_listen);
      sdp_close(session);
      //exit(EXIT_FAILURE);
      return -1;
    } 
  Buffer[length] = 0;
  LogInfo( iLtpLogId, "Received:"<<Buffer);
  #endif //SEND_TO_CLIENT

    // Close this acceptted client connection.
    status = close(fd_socket_connect);
    if (status < 0)
    {
      LogError( iLtpLogId, "Close client socket error");
      close(fd_socket_listen);
      //exit(EXIT_FAILURE);
      return -1;
    }
    //sleep(1);
  }

  // The service record will stay registered and advertised until the program exits or 
  // closes the connection to the local SDP server by calling sdp close.
  sdp_close(session);
  LogInfo( iLtpLogId, "Advertising new services STOP!");
  // Close listenning server connection.
  status = close(fd_socket_listen);
  if (status < 0)
  {
    LogError( iLtpLogId, "close client socket error");
    //exit(EXIT_FAILURE);
    return -1;
  }
  
  return 0;
}

int CLtpBluetoothSocketServer::ServerSocketOpen(unsigned short aPort, int aBacklog)
{
  int status = 0;

  // Create an socket endpoint for communication
  iFdSocketListen = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (iFdSocketListen < 0)
  {
    LogError( iLtpLogId, "Socket create failed");
    return 0;//exit(EXIT_FAILURE);
  }
  
  // Bind a name to a BT socket port
  struct sockaddr_rc serv_addr;
  int port = 0;
  
  serv_addr.rc_family = AF_BLUETOOTH;
  #ifdef __cplusplus
  serv_addr.rc_bdaddr = my_BDADDR_ANY;
  #else //__cplusplus
  serv_addr.rc_bdaddr = *BDADDR_ANY;
  #endif //__cplusplus
  
  if (0 == aPort)
  {
    for (port = BT_SOCKET_MIN_PORT; port <= BT_SOCKET_MAX_PORT; port++)
    {
      serv_addr.rc_channel = port;
      status = bind(iFdSocketListen, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
      if(status < 0)
      {
        LogError( iLtpLogId, "Socket bind failed");
      }
      if(status == 0)
        break;
    }
    if (port <= BT_SOCKET_MAX_PORT)
    {
      LogInfo( iLtpLogId, "Socket dynamic bind to port:"<<serv_addr.rc_channel);
    }
    else
    {
      LogInfo( iLtpLogId, "Dynamic bind port failed!");
      return 0;//exit(EXIT_FAILURE);
    }
  }
  else
  {
    port = aPort;
    serv_addr.rc_channel = port;
    status = bind(iFdSocketListen, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(status < 0)
    {
      LogError( iLtpLogId, "Socket bind fixed port failed");
      return 0;//exit(EXIT_FAILURE);
    }
    LogInfo( iLtpLogId, "bind to fixed port#"<<serv_addr.rc_channel);
  }

  // Register the "TT_BTService" service.
  iSession = register_service(port);
  if (NULL == iSession)
  {
    LogInfo( iLtpLogId, "Advertising new services Failed ...");
    return 0;
  }
  LogInfo( iLtpLogId, "Advertising new services START ...");

  // Listen for connections on th socket iFdSocketListen.
  // BT_SOCKET_CONN_BACKLOG = The maximum length to which the queue of pending connections.
  status = listen(iFdSocketListen, aBacklog); 
  if(status < 0)
  {
    LogError( iLtpLogId, "Socket listen failed");
    close(iFdSocketListen);
    sdp_close(iSession);
    return 0;//exit(EXIT_FAILURE);
  }
  return 1;
}

int CLtpBluetoothSocketServer::ServerSocketAccept()
{
  LogInterface( iLtpLogId, "");
  
  int sin_size; 
  struct sockaddr_rc client_addr; 

  // Accept a connection on a socket.
  // It extracts the first connection request on the queue of pending connections for the listening socket,
  // creates a new connected socket, and returns a new file descriptor referring to that socket.  
  // The newly created socket is not in the listening state.  
  // The original socket sockfd is unaffected by this call.
  // It blocks the caller until a connection is present. 
  LogInfo( iLtpLogId, "ready to accept...");
  //iFdSocketConnect = accept(iFdSocketListen, (struct sockaddr*)NULL, NULL); 
  sin_size = sizeof(struct sockaddr_rc);
  iFdSocketConnect = accept(iFdSocketListen, (struct sockaddr*)&client_addr, (socklen_t*)&sin_size); 
  if(iFdSocketConnect < 0)
  {
    LogError( iLtpLogId, "Socket client accept error");
    //close(iFdSocketListen);
    //sdp_close(iSession);
    return 0;//exit(EXIT_FAILURE);
  }

  CLtpCommonTools::readOneLineBufInit(iFdSocketConnect, &sgRlBufRead);

  // Retrieve the address string from acceptted client_addr.
  char addr_str[64];
  memset(addr_str, 0, sizeof(addr_str));
  ba2str(&client_addr.rc_bdaddr, addr_str);
  LogInfo( iLtpLogId, "Accept connection from addr:"<<addr_str);
  
  return 1;
}

int CLtpBluetoothSocketServer::ServerSocketRead(char** aBuffer)
{
  LogInterface( iLtpLogId, "");

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
    //close(iFdSocketListen);
    //sdp_close(iSession);
    return 0;//exit(EXIT_FAILURE);
  }
  iBuffer[KMessageBufUnitSize-1] = '\0'; /* null character manually added at buffer end */
  iBuffer[length] = '\0';
  LogInfo( iLtpLogId, "Received length="<<length);

  //CLtpCommonTools::printBufHex(iBuffer, 200/*KMessageBufUnitSize*/);

  *aBuffer = iBuffer;

  return 1;
}

int CLtpBluetoothSocketServer::ServerSocketWrite(const char* aBuffer)
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
  
    if (KMessageBufUnitSize < (aBuffer_strlen + 2)) // reserve 1 byte for '\n'+'\0'.
    {
      LogInfo( iLtpLogId, "Character string too long! Current allocated buffer size = "<< KMessageBufUnitSize);
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
    LogError( iLtpLogId, "Socket write error");
    //close(iFdSocketConnect);
    return 0;//exit(EXIT_FAILURE);
  } 
  LogInfo( iLtpLogId, "Write length = \n"<< length);

  return 1;
}

int CLtpBluetoothSocketServer::ServerSocketClose()
{
  LogInterface( iLtpLogId, "");
  int status = 0;
  
  // Close this socket connection.
  status = close(iFdSocketConnect);
  if (status < 0)
  {
    LogError( iLtpLogId, "close connection socket error");
    return 0;//exit(EXIT_FAILURE);
  }

  // The service record will stay registered and advertised until the program exits or 
  // closes the connection to the local SDP server by calling sdp close.
  status = sdp_close(iSession);
  if (status < 0)
  {
    LogError( iLtpLogId, "sdp_close session error");
    return 0;//exit(EXIT_FAILURE);
  }
 LogInfo( iLtpLogId, "Advertising new services STOP!");
  
  // Close listenning server connection.
  status = close(iFdSocketListen);
  if (status < 0)
  {
    LogError( iLtpLogId, "close listenning socket error");
    return 0;//exit(EXIT_FAILURE);
  }

  return 1;
}

int CLtpBluetoothSocketServer::test(int argc, char **argv)
{
#if 0
  return server_start(argc, argv);
#else
  #if 0
  // Open a listenning socket and start acceptting.
  if (!ServerSocketOpen(0, BT_SOCKET_CONN_BACKLOG))
    printf("Err ServerSocketOpen\n");
  #endif

  //  Start acceptting.
  if (!ServerSocketAccept())
  {
    printf("Err ServerSocketAccept\n");
  }
  
  // Test Read -> loop write.
  char* aBuffer = NULL;
  if (!ServerSocketRead(&aBuffer))
  {
    printf("Err ServerSocketRead\n");
  }
  
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


