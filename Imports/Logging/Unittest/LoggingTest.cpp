//
// This file contain Logging UnitTest for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "ILogging.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;

logging::TLogComponent iLtpLogId;
logging::TLogComponent iLtpLogId2;

class LoggingTest: public ::testing::Test
{
};

// This Unit Test assume TCP/IP Socket as the underlying physical connection by specifying ESocketType_IP. 
TEST_F(LoggingTest, HowToUseLoggingTest)
{
  std::cout << "HowToUseLoggingTest unit testing Start!!!" << std::endl;
  
  logging::IControl().RegisterComponent( "ExternalHandler", iLtpLogId);
  LogInterface( iLtpLogId, "");
  LogInterface( iLtpLogId, "aaa:" << 123 << " ,bbb:" << "text" << " ,ccc:" << 25.48);

  logging::IControl().RegisterComponent( "UserDataService", iLtpLogId2);
  LogInterface( iLtpLogId2, "");
  LogInterface( iLtpLogId2, "aaa:" << 123 << " ,bbb:" << "text" << " ,ccc:" << 25.48);
  
  std::cout << "HowToUseLoggingTest unit testing Done!!!" << std::endl;
}


