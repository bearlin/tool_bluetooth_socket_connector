//
// This file contain LtpDemo using gtest/gmock framework.
//

#include "LtpConnection.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */

using namespace NBsc;

// Implement this listener for SP to get the location.
//--------------------------------------------------------------------------------------
class CBscDemoConnectionListener : public ILtpConnectionListener
{
public:
  void messageReceived(const std::string& message)
  {
    printf("CBscDemoConnectionListener messageReceived: %s \n", message.c_str());
  }
};

//--------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
  std::string instance = "HU: "; //default as HU instance.
  TSocketType SocketType = ESocketType_IP;
  
  if(argc != 3)   
  {
    printf("usage: %s <0:HU|1:SP> <0:Use IP Socket|1:Use BT Socket>\n", argv[0]);    
    return -1;  
  }

  if ('0' == *argv[1])
    instance = "HU: ";
  else if ('1' == *argv[1])
    instance = "SP: ";

  if ('0' == *argv[2])
    SocketType = ESocketType_IP;
  else if ('1' == *argv[2])
    SocketType = ESocketType_BT;

  std::cout << "BscDemo main() start!!!" << std::endl;
  std::cout << "LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;
  
  if (instance == std::string("SP: ")) 
  {
    printf("Press ENTER to send a request, press 'q' to disconnect and exit program.");
    
    // Create client socket, then connect to server socket.
    CLtpConnection* pSPconnection = new CLtpConnection(instance, SocketType);
    
    // Client will entering a loop to prevent close the socket connection.
    while ('q' != getchar()) 
    {
      // One time socket write().
      std::string str = CLtpTimestampedLocation::GetLocalTimeStamp();
      std::cout<<"str:"<<str<<std::endl;
      pSPconnection->blockSocketSendMessage(instance, str.c_str());
      std::cout<<"Sent!"<<std::endl;
    }  

    delete pSPconnection;
  }
  else if (instance == std::string("HU: ")) 
  {
    CBscDemoConnectionListener HuConnectionListener;
    // Create HU connection = create server socket, then bind()/listen() for the client socket.
    CLtpConnection* pHUconnection = new CLtpConnection(instance, SocketType);
    pHUconnection->registerListener(instance, HuConnectionListener);
      
    while (pHUconnection->startConnectorSocketServerLoop(0))
    {
    }
    
    delete pHUconnection;
  }
  else 
  {
    //throw CLtpIllegalArgumentException("Unknown instance: " + instance);
    std::cout << "Unknown instance: " + instance << std::endl;
  }

  std::cout << "BscDemo main() Done!!!" << std::endl;
  return 0;
}

