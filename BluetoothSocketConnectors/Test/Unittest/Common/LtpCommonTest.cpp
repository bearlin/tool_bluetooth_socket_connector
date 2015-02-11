//
// This file contain UnitTest of NBsc::Common for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <stdio.h>

using namespace NBsc;
using ::testing::Return;
using ::testing::AtLeast;
using ::testing::_;


class CLtpJsonToolsTest: public ::testing::Test
{
};

class CLtpExceptionTest: public ::testing::Test
{
};

class CLtpTimerTest: public ::testing::Test
{
};

class CPThreadTest: public ::testing::Test
{
};

class MyThread : public CPThread
{
public:
  void *run() 
  {
    for (int i = 0; i < 5; i++) 
    {
      printf("thread %lu running - %d\n",  (long unsigned int)self(), i+1);
      sleep(2);
    }
    printf("thread done %lu\n", (long unsigned int)self());
    
    return NULL;
  }
};

TEST_F(CLtpJsonToolsTest, QueringObjectMembersAndTypes)
{
  // Test messages shoud be a JSON Object:
  //{"ptl_ver":{"major":9,"minor":6},"msg_type":"RESP","msg_id":0,"method":"","resp_code":1,"parameters":[]}
  std::string message = "{\"ptl_ver\":{\"major\":9,\"minor\":6},\"msg_type\":\"RESP\",\"msg_id\":0,\"method\":\"\",\"resp_code\":1,\"parameters\":[]}";
  rapidjson::Document iJsonParser;

  // Use rapidjson::Document to parse this message.
  if (iJsonParser.Parse<0>(message.c_str()).HasParseError())// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
  {
    EXPECT_TRUE(0U); //Parse failed, so force this test as failed.
    std::cout << "Message is not a valid JSON document" << std::endl;
    throw CLtpJsonParseException("Not a valid JSON document");
  }

  // This Document is also a JSON Object, so we can use QueringObjectMembersAndTypes() to loop through its structures and types.
  EXPECT_TRUE(1 == CLtpJsonTools::QueringObjectMembersAndTypes(iJsonParser));
}

TEST_F(CLtpJsonToolsTest, GetLocalTimeStamp)
{
  std::string timestamp;
  timestamp = CLtpTimestampedLocation::GetLocalTimeStamp();
  //printf("timestamp : %s\n", timestamp.c_str());
  
  EXPECT_TRUE(0 != timestamp.length());
}

TEST_F(CLtpJsonToolsTest, TLocationType_TypesToJsonValue_And_JsonValueToString)
{
  std::string actual_msg;
  std::string expect_msg;
  std::vector<TLocationType> locationTypes;
  
  //----------------------------------------------------------------------
  locationTypes.clear();
  //locationTypes.push_back(ELocationType_DESTINATION);
  //locationTypes.push_back(ELocationType_LAST_KNOWN_CAR_POSITION);

  // TypesToJsonValue
  expect_msg = "[[]]";
  rapidjson::Document document1;// Set this value as an empty object.
  document1.SetObject();
  rapidjson::Value json1;// Set this value as an empty array.
  json1.SetArray();
  json1.Clear();
  CLtpJsonTools::TypesToJsonValue(locationTypes, document1, json1);
  actual_msg = CLtpJsonTools::JsonValueToString(json1);
  
  //printf("TLocationType_TypesToJsonValue_And_JsonValueToString(Empty JSON Array) :\n");
  //printf("expect_msg : %s\n", expect_msg.c_str());
  //printf("actual_msg : %s\n", actual_msg.c_str());
  EXPECT_TRUE(0 == expect_msg.compare(actual_msg));
  //----------------------------------------------------------------------
  
  //----------------------------------------------------------------------
  locationTypes.clear();
  locationTypes.push_back(ELocationType_DESTINATION);
  locationTypes.push_back(ELocationType_LAST_KNOWN_CAR_POSITION);

  // TypesToJsonValue
  expect_msg = "[[\"DESTINATION\",\"LAST_KNOWN_CAR_POSITION\"]]";
  rapidjson::Document document2;// Set this value as an empty object.
  document2.SetObject();
  rapidjson::Value json2;// Set this value as an empty array.
  json2.SetArray();
  json2.Clear();
  CLtpJsonTools::TypesToJsonValue(locationTypes, document2, json2);
  actual_msg = CLtpJsonTools::JsonValueToString(json2);
  
  //printf("TLocationType_TypesToJsonValue_And_JsonValueToString([0,1] JSON Array) :\n");
  //printf("expect_msg : %s\n", expect_msg.c_str());
  //printf("actual_msg : %s\n", actual_msg.c_str());
  EXPECT_TRUE(0 == expect_msg.compare(actual_msg));
  //----------------------------------------------------------------------
}

TEST_F(CLtpExceptionTest, ExceptionClassesTest)
{
  std::string StrIllegalArgumentException;

  // For CLtpBaseException.
  try
  {
    throw CLtpBaseException();
  }
  catch(CLtpBaseException& e)
  {
    // The catch should be here! And the (e.what()) description should be the same with (e.getExceptionPrefixStr() + StrIllegalArgumentException).
    EXPECT_TRUE(0 == std::string(e.getExceptionPrefixStr()).compare(e.what()));
  }
  catch(...)
  {
    // The catch should NOT be here! So force to assert failed.
    EXPECT_TRUE(0U); //force to assert failed.
  }
  
  // For CLtpIllegalArgumentException.
  StrIllegalArgumentException = "Exception CLtpIllegalArgumentException !";
  try
  {
    throw CLtpIllegalArgumentException(StrIllegalArgumentException);
  }
  catch(CLtpBaseException& e)
  {
    // The catch should be here! And the (e.what()) description should be the same with (e.getExceptionPrefixStr() + StrIllegalArgumentException).
    EXPECT_TRUE(0 == (e.getExceptionPrefixStr() + StrIllegalArgumentException).compare(e.what()));
  }
  catch(...)
  {
    // The catch should NOT be here! So force to assert failed.
    EXPECT_TRUE(0U); //force to assert failed.
  }

  // For CLtpJsonParseException.
  StrIllegalArgumentException = "Exception CLtpJsonParseException !";
  try
  {
    throw CLtpJsonParseException(StrIllegalArgumentException);
  }
  catch(CLtpBaseException& e)
  {
    // The catch should be here! And the (e.what()) description should be the same with (e.getExceptionPrefixStr() + StrIllegalArgumentException).
    EXPECT_TRUE(0 == (e.getExceptionPrefixStr() + StrIllegalArgumentException).compare(e.what()));
  }
  catch(...)
  {
    // The catch should NOT be here! So force to assert failed.
    EXPECT_TRUE(0U); //force to assert failed.
  }

  // For CLtpRuntimeException.
  StrIllegalArgumentException = "Exception CLtpRuntimeException !";
  try
  {
    throw CLtpRuntimeException(StrIllegalArgumentException);
  }
  catch(CLtpBaseException& e)
  {
    // The catch should be here! And the (e.what()) description should be the same with (e.getExceptionPrefixStr() + StrIllegalArgumentException).
    EXPECT_TRUE(0 == (e.getExceptionPrefixStr() + StrIllegalArgumentException).compare(e.what()));
  }
  catch(...)
  {
    // The catch should NOT be here! So force to assert failed.
    EXPECT_TRUE(0U); //force to assert failed.
  }

  // For other types.
  int err_no = 20;
  try
  {
    throw err_no;
  }
  catch(int e)
  {
    // The catch should be here! And the 'e' should be the same with err_no.
    EXPECT_TRUE(err_no == e);
  }
  catch(...)
  {
    // The catch should NOT be here! So force to assert failed.
    EXPECT_TRUE(0U); //force to assert failed.
  }

  const char* err_pct = "asdfasdf";
  try
  {
    throw err_pct;
  }
  catch(const char* e)
  {
    EXPECT_TRUE(0 == std::string(err_pct).compare(e));
  }
  catch(...)
  {
    EXPECT_TRUE(0U); //force to assert failed.
  }

  float err_float = (0.5);
  try
  {
    throw err_float;
  }
  catch(float e)
  {
    EXPECT_TRUE(err_float == e);
  }
  catch(...)
  {
    EXPECT_TRUE(0U); //force to assert failed.
  }
}

// ------------------------ CLtpTimerTest ------------------------
// signal handler for timer object 1.
void fn_timer_expire_handler1(int sig, siginfo_t* si, void* uc)
{
  static int countA=0;
  countA++;
  
  /* UNSAFE: This handler uses non-async-signal-safe functions printf() */
  printf("fn_timer_expire_handler1()\n");
  printf("[%s] Got signal %d, countA:%d\n", CLtpTimestampedLocation::GetLocalTimeStamp().c_str(), sig, countA);
}

// signal handler for timer object 2.
void fn_timer_expire_handler2(int sig, siginfo_t* si, void* uc)
{
  static int countB=0;
  countB++;
  
  /* UNSAFE: This handler uses non-async-signal-safe functions printf() */
  printf("fn_timer_expire_handler2()\n");
  printf("[%s] Got signal %d, countB:%d\n", CLtpTimestampedLocation::GetLocalTimeStamp().c_str(), sig, countB);
}

TEST_F(CPThreadTest, CPThreadMain)
{
  MyThread* thread1 = new MyThread();
  MyThread* thread2 = new MyThread();
  thread1->start();
  thread2->start();
  thread1->join();
  thread2->join();
  printf("CPThreadTest done\n");
}

TEST_F(CLtpTimerTest, CLtpTimerCreateStartDelete)
{
  //--------------------------------------------------
  // new two objects.
  CLtpTimer* pLtpTimer1 = new CLtpTimer();
  CLtpTimer* pLtpTimer2 = new CLtpTimer();
  EXPECT_TRUE(NULL != pLtpTimer1);
  EXPECT_TRUE(NULL != pLtpTimer2);

  // Create the timer.
  //EXPECT_TRUE(pLtpTimer1->TimerCreate(NULL, SIG_TIMER1));                     //use internal SignalHandler function.
  EXPECT_TRUE(pLtpTimer1->TimerCreate(fn_timer_expire_handler1, SIG_TIMER1));   //use custom SignalHandler function.
  //EXPECT_TRUE(pLtpTimer2->TimerCreate(NULL, SIG_TIMER2));                     //use internal SignalHandler function.
  EXPECT_TRUE(pLtpTimer2->TimerCreate(fn_timer_expire_handler2, SIG_TIMER2));   //use custom SignalHandler function.

  // Start the timer.
  EXPECT_TRUE(pLtpTimer1->TimerStart(1, 500000000UL, 0, 500000000UL));
  EXPECT_TRUE(pLtpTimer2->TimerStart(1, 500000000UL, 3, 0));

  // Waiting for the timer signals.
#if 1
  EXPECT_TRUE(CLtpTimer::GoNanoSleeper(10));
#else
  /* Do busy work. */
  printf("busying... busy until user input 'q' or after 10 seconds\n");
  while ('q' != getchar())
  {
    //busy until user input 'q' or after 10 seconds.
  };
#endif

  // Stop timer objects.
  EXPECT_TRUE(pLtpTimer1->TimerStop());
  EXPECT_TRUE(pLtpTimer2->TimerStop());
  printf("Stop timer objects\n");

  EXPECT_TRUE(CLtpTimer::GoNanoSleeper(3));

  // Delete timer objects.
  EXPECT_TRUE(pLtpTimer1->TimerDelete());
  EXPECT_TRUE(pLtpTimer2->TimerDelete());
  printf("That's all folks\n");

  EXPECT_TRUE(CLtpTimer::GoCountDownSleeper(5));

  delete pLtpTimer1;
  delete pLtpTimer2;
  //--------------------------------------------------
}
// ------------------------ CLtpTimerTest ------------------------


