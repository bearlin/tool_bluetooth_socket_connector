//
// This file contain CLtpConnection UnitTest for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "MockIBscConnectionListener.h"

#include <stdio.h>

using namespace NBsc;
using ::testing::Return;
using ::testing::AtLeast;
//using ::testing::TypedEq;

std::string messageREQ = "{\"ptl_ver\":{\"major\":1,\"minor\":0},\"msg_type\":\"REQ\",\"msg_id\":9,\"method\":\"getNavData\",\"parameters\":{\"a\":\"b\",\"c\":\"d\"}}";
std::string messageRESP = "{\"ptl_ver\":{\"major\":1,\"minor\":0},\"msg_type\":\"RESP\",\"msg_id\":0,\"method\":\"\",\"resp_code\":1}";

class MyThreadSP : public CPThread
{
public:
  void *run() 
  {
    printf("thread %lu running\n",  (long unsigned int)self());
    
    std::string instance = "SP: ";
    std::cout << instance + " thread start at LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;

    // Create SP connection = create client socket, then connect to server socket.
    CLtpConnection* pSPconnection = new CLtpConnection(instance, ESocketType_IP);

    // One time socket write().
    pSPconnection->blockSocketSendMessage(instance, messageREQ);

    delete pSPconnection;
    
    printf("thread done %lu\n", (long unsigned int)self());
    return NULL;
  }
};

class MyThreadHU : public CPThread
{
public:
  void *run() 
  {
    printf("thread %lu running\n",  (long unsigned int)self());
    
    std::string instance = "HU: ";
    std::cout << instance + " thread start at LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;

    CConnectionListenerMock HuConnectionListenerMock;
    // Create HU connection = create server socket, then bind()/listen() for the client socket.
    CLtpConnection* pHUconnection = new CLtpConnection(instance, ESocketType_IP);
    pHUconnection->registerListener(instance, HuConnectionListenerMock);

    // When SP write a string to HU, low level socket write API will auto append "\n" to string end(if the original string is not end with "\n"), 
    // this "\n" is required for Android socket read API as a message ending.
    EXPECT_CALL(HuConnectionListenerMock, messageReceived(messageREQ+"\n"))
      .Times(AtLeast(1))
      .WillRepeatedly(Return());

    // Wait for one time socket read().
    // We don't want HU socket server entering a infinity accept() -> read()/write() loop, so just specify that it will just do this loop once.
    pHUconnection->startConnectorSocketServerLoop(1);

    // Wait for a short time to prevent disconnect(destruct pHUconnection will disconnect) before SP read() RESP from socket.
    std::cout << "sleep(2) after startConnectorSocketServerLoop(1)" << std::endl;
    sleep(2);

    delete pHUconnection;
    
    printf("thread done %lu\n", (long unsigned int)self());
    return NULL;
  }
};

class CLtpConnectionTest: public ::testing::Test
{
};

TEST_F(CLtpConnectionTest, CLtpConnectionConstructorAndDestructor)
{
  CLtpConnection* pconnectionImpl = new CLtpConnection("HU: ", ESocketType_IP);

  EXPECT_TRUE(pconnectionImpl != NULL);
  
  delete pconnectionImpl;
}

TEST_F(CLtpConnectionTest, CLtpConnectionblockSocketSendReceiveMessages)
{
  std::cout << "CLtpConnectionTest unit testing Start!!!" << std::endl;
  std::cout << "LocalTimeStamp():" << CLtpTimestampedLocation::GetLocalTimeStamp() << std::endl;

  MyThreadHU* threadHU = new MyThreadHU();
  MyThreadSP* threadSP = new MyThreadSP();
  
  threadHU->start();

  // Wait for HU thread ready and running.
  std::cout << "sleep(1)" << std::endl;
  sleep(1);
  
  threadSP->start();

  // Wait for everything done.
  std::cout << "sleep(1)" << std::endl;
  sleep(1);
  
  //threadHU->join();
  //threadSP->join();
  
  std::cout << "CLtpConnectionTest unit testing Done!!!" << std::endl;
}

