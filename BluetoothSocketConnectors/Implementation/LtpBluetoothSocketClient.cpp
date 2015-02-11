//
// This file contain CLtpBluetoothSocketClient implementation for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpBluetoothSocketClient.h"
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
  extern bdaddr_t my_BDADDR_ANY;// = {0};
#endif //__cplusplus

using namespace NBsc;

extern logging::TLogComponent iLtpLogId;

static struct TReadOneLineBuf sgRlBufRead;

CLtpBluetoothSocketClient::CLtpBluetoothSocketClient(std::string instance) : 
  iInstance(instance), 
  iFdSocketConnect(0),
  iBuffer(NULL)
{
  iBuffer = new char[KMessageBufUnitSize];
  memset(iBuffer, '\0', KMessageBufUnitSize);
}

CLtpBluetoothSocketClient::~CLtpBluetoothSocketClient()
{
  if (iBuffer)
    delete []iBuffer;
}

int CLtpBluetoothSocketClient::search_service(const char *bt_addr)
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

  int status, status2;
  bdaddr_t target;
  uuid_t svc_uuid;
  sdp_list_t *response_list, *search_list, *attrid_list;
  sdp_session_t *session = 0;
  uint32_t range = 0x0000ffff;
  uint8_t port = 0;
  
  char string[128] = {0};

  LogInfo( iLtpLogId, "------------ search_service start ------------");
  
  if(bt_addr == NULL)
  {
    fprintf(stderr, "Missing <bt_addr>\n");
    //exit(EXIT_FAILURE);
    return -1;
  }
  str2ba( bt_addr, &target );

  // Connect to the SDP server running on the remote machine
#ifdef __cplusplus
  session = sdp_connect( &my_BDADDR_ANY, &target, 0 );
#else //__cplusplus
  session = sdp_connect( BDADDR_ANY, &target, 0 );
#endif //__cplusplus
  if (!session) 
  {
    LogInfo( iLtpLogId, "Failed to connect to the local SDP server. "<< errno);
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "sdp_connect OK!");

  sdp_uuid128_create( &svc_uuid, &svc_uuid_int );
  search_list = sdp_list_append( 0, &svc_uuid );
  attrid_list = sdp_list_append( 0, &range );

  // Get a list of service records that have UUID 0xabcd
  response_list = NULL;
  status = sdp_service_search_attr_req( session, search_list, SDP_ATTR_REQ_RANGE, attrid_list, &response_list);

  if (response_list == NULL)
  {
    LogInfo( iLtpLogId, "response_list not found!");
  }
    
  if (status == 0)
  {
    sdp_list_t *proto_list = NULL;
    sdp_list_t *r = response_list;

    // Go through each of the service records
    for (; r; r = r->next ) 
    {
      // Get record.
      sdp_record_t *rec = (sdp_record_t*) r->data;

      // Get service name.
      status2 = sdp_get_service_name( rec, string, sizeof(string) );
      if (status2 < 0)
      {
        LogInfo( iLtpLogId, "sdp_get_service_name error");
      }
      else
      {
        LogInfo( iLtpLogId, "service_name:"<< string);
      }
      
      // Get service description.
      status2 = sdp_get_service_desc( rec, string, sizeof(string) );
      if (status2 < 0)
      {
        LogInfo( iLtpLogId, "sdp_get_service_desc error");
      }
      else
      {
        LogInfo( iLtpLogId, "service_desc:"<<string);
      }
      
      // Get service provider name.
      status2 = sdp_get_provider_name( rec, string, sizeof(string) );
      if (status2 < 0)
      {
        LogInfo( iLtpLogId, "sdp_get_provider_name error");
      }
      else
      {
        LogInfo( iLtpLogId, "provider_name:"<< string);
      }
      
      // Get a list of the protocol sequences
      status2 = sdp_get_access_protos( rec, &proto_list );
      if (status2 < 0)
      {
        // Cleanup and exit.
        LogInfo( iLtpLogId, "sdp_get_access_protos error");
        sdp_list_free( response_list, 0 );
        sdp_list_free( search_list, 0 );
        sdp_list_free( attrid_list, 0 );
        sdp_close( session );
        //exit(EXIT_FAILURE);
        return -1;
      }
      
      // Get RFCOMM port number
      port = sdp_get_proto_port( proto_list, RFCOMM_UUID ); 
      if (port <= 0)
      {
        // Cleanup and exit.
        LogInfo( iLtpLogId, "sdp_get_proto_port RFCOMM_UUID error");
        sdp_list_free( proto_list, 0 );
        sdp_list_free( response_list, 0 );
        sdp_list_free( search_list, 0 );
        sdp_list_free( attrid_list, 0 );
        sdp_close( session );
        //exit(EXIT_FAILURE);
        return -1;
      } 
      else 
      {
        LogInfo( iLtpLogId, "found service running on RFCOMM port:"<< port);
      }
      
      // Free resources.
      sdp_list_free( proto_list, 0 );
      sdp_record_free( rec );
    }
  }
  else
  {
    LogInfo( iLtpLogId, "The request completed unsuccessfully due to a timeout!");
  }
  
  // Cleanup and return RFCOMM port number.
  sdp_list_free( response_list, 0 );
  sdp_list_free( search_list, 0 );
  sdp_list_free( attrid_list, 0 );
  sdp_close( session );

  LogInfo( iLtpLogId, "------------ search_service end ------------");
  return port;
}

int CLtpBluetoothSocketClient::client_start(int argc, char **argv)
{
  //LogInterface( logId, "");
  int status = 0;

  // The socket server BT address will be passed into this function via argv[].
  char dest[18] = "1C:4B:D6:0F:F1:56"; //BuildServerTest-linstNB
  if(argc != 2)
  {
    LogInfo( iLtpLogId, "\n Usage: "<<argv[0]<<" <bt_addr> \n");
    return -1;
  } 
  memset(dest, 0, 18);
  strncpy(dest, argv[1], 17);
  LogInfo( iLtpLogId, "<bt_addr>:"<<argv[1]);

  // Create an socket endpoint for communication
  int fd_socket = 0;//, n = 0;
  fd_socket = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if (fd_socket < 0)
  {
    LogInfo( iLtpLogId, "Socket create failed");
    //exit(EXIT_FAILURE);
    return -1;
  }
  LogInfo( iLtpLogId, "Socket create ok!");

  // Initiate a connection to server socket at port returned by search_service()
  struct sockaddr_rc serv_addr = { 0 };
  memset(&serv_addr, '0', sizeof(serv_addr)); 
  serv_addr.rc_family = AF_BLUETOOTH;
  serv_addr.rc_channel = search_service(argv[1]);
  status = str2ba( dest, &serv_addr.rc_bdaddr ); //convert between strings and bdaddr_t structures
  if(status < 0)
  {
    LogInfo( iLtpLogId, "Socket str2ba failed");
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
  LogInfo( iLtpLogId, "Received:"<< Buffer);

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
    LogInfo( iLtpLogId, "Write "<<status<<" bytes done");
  }

  LogInfo( iLtpLogId, "Press any key to close connection...");
  if (EOF == getchar())
  {
    // Close this socket connection.
    close(fd_socket);
    return 0;
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

int CLtpBluetoothSocketClient::ClientSocketOpen()
{

  // Create an socket endpoint for communication
  iFdSocketConnect = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  if(iFdSocketConnect < 0)
  {
    LogInfo( iLtpLogId, "Socket create failed");
    return 0;
  }
  //printf("Socket create ok!\n");
  
  return 1;
}

int CLtpBluetoothSocketClient::ClientSocketConnect(const char* aAddr, unsigned short aPort)
{
  int status = 0;
  
  // Initiate a connection to server socket at port returned by search_service()
  struct sockaddr_rc serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr)); 
  serv_addr.rc_family = AF_BLUETOOTH;
  if (0 == aPort)
    serv_addr.rc_channel = search_service(aAddr);
  else
    serv_addr.rc_channel = aPort;
  
  str2ba(aAddr, &serv_addr.rc_bdaddr); //convert between strings and bdaddr_t structures

  status = connect(iFdSocketConnect, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); 
  if (status < 0)
  {
    LogInfo( iLtpLogId, "ClientSocketConnect failed!");
    return 0;
  }

  CLtpCommonTools::readOneLineBufInit(iFdSocketConnect, &sgRlBufRead);

  LogInfo( iLtpLogId, "Socket connect to Addr("<<aAddr<<") at Port:"<< serv_addr.rc_channel);
  
  return 1;
}

int CLtpBluetoothSocketClient::ClientSocketRead(char** aBuffer)
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
  LogInfo( iLtpLogId, "Received ("<<length<<") bytes:"<< iBuffer);

  //CLtpCommonTools::printBufHex(iBuffer, 200/*KMessageBufUnitSize*/);

  *aBuffer = iBuffer;

  return 1;
}

int CLtpBluetoothSocketClient::ClientSocketWrite(const char* aBuffer)
{

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
      LogInfo( iLtpLogId, "Character string too long! Current allocated buffer size ="<< KMessageBufUnitSize);
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
  LogInfo( iLtpLogId, "Write ("<<length<<") bytes:"<< iBuffer);

  return 1;
}

int CLtpBluetoothSocketClient::ClientSocketClose()
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

int CLtpBluetoothSocketClient::test(int argc, const char **argv)
{
#if 0
  return client_start(argc, argv);
#else
  #if 0
  // Open a connection socket.
  if (!ClientSocketOpen())
    printf("Err ClientSocketOpen\n");
  if (!ClientSocketConnect(BT_SOCKET_SERVER_ADDR, 0))
    printf("Err ClientSocketConnect\n");
  #endif

  // Test Write -> loop read.
  if (!ClientSocketWrite(NULL))
    LogInfo( iLtpLogId, "Err ClientSocketWrite");

  while (1)
  {
    char* aBuffer = NULL;
    if (!ClientSocketRead(&aBuffer))
      LogInfo( iLtpLogId, "Err ClientSocketRead");
  }

  #if 0
  // Close the socket.
  if (!ClientSocketClose())
    printf("Err ClientSocketClose\n");
  #endif

  return 1;
#endif

}


