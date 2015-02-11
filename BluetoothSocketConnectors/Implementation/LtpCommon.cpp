//
// This file contain Common datas for Location Information Transfer Over IP/Bluetooth Socket.
//

#include "LtpCommon.h"
#include "LtpExceptions.h"

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>       /* time_t, struct tm, time, localtime, strftime */
#include <unistd.h>

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  // Global const variables, other cpp files can just "extern" below variables in order to use them.
  //-------------------------------------------------------------------------------------------------------
  // Message types list:
  const char* Ltp_msg_types_req = "REQ";
  const char* Ltp_msg_types_resp = "RESP";

  // Message key strings:
  const char* Ltp_ptl_ver = "ptl_ver";
  const char* Ltp_major = "major";
  const char* Ltp_minor = "minor";
  const char* Ltp_msg_type = "msg_type";
  const char* Ltp_msg_id = "msg_id";
  const char* Ltp_method = "method";
  const char* Ltp_resp_code = "resp_code";
  const char* Ltp_parameters = "parameters";

  // Methods list:
  const char* Ltp_methods_TestFunc = "TestFunc";
  const char* Ltp_methods_getUserData = "getUserData";

  // Method "getNavData" parameters:
  const char* Ltp_methods_getUserData_param_location_type = "location_type";
  const char* Ltp_methods_getUserData_param_timestamp = "timestamp";
  const char* Ltp_methods_getUserData_param_latitude = "latitude";
  const char* Ltp_methods_getUserData_param_longitude = "longitude";
  const char* Ltp_methods_getUserData_param_coordinates = "coordinates";
  
  // Method "getNavData" type strings:
  const char* Ltp_methods_getUserData_param_type_dest = "DESTINATION";
  const char* Ltp_methods_getUserData_param_type_lkcp = "LAST_KNOWN_CAR_POSITION";

  // rapidjson types.
  const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };

  const int KMessageBufUnitSize = 0x400;
  //-------------------------------------------------------------------------------------------------------

  std::string CLtpTimestampedLocation::GetLocalTimeStamp()
  {
    // Timestamps are presented as strings using a restricted ISO8601 format: 
    // YYYY-MM-DDThh:mm:ss.SSSZ ('T' and 'Z' are literal.). 
    // EX: "1997-07-16T19:20:30.450+0100".
    // EX: "2014-02-19T00:01:07.000+0800"
    // Check http://www.w3.org/TR/NOTE-datetime, http://www.cl.cam.ac.uk/~mgk25/iso-time.html
    // Check http://www.cplusplus.com/reference/ctime/strftime/ about how to create this timestamp format.
    std::string timestamp;
    
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[512];

    memset(buffer, 0, sizeof(buffer));
    
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%FT%T.000%z", timeinfo);
    //printf("%s\n", buffer);
    timestamp = buffer;
    
    return timestamp;
  }

  int CLtpJsonTools::QueringObjectMembersAndTypes(const rapidjson::Value& json)
  {
    if (json.IsObject())
    {
      printf("***--------------***\n");
      for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); ++itr)
        printf("Type of member \"%s\" : %s\n", itr->name.GetString(), kTypeNames[itr->value.GetType()]);
      printf("***--------------***\n");
      return 1;
    }
    else
    {
      printf("Can't QueringObjectMembersAndTypes!\n");
      //throw CLtpRuntimeException("Illegal JSON Value, must start from Object {...}");
      return 0;
    }
  }

  // Convert a rapidjson::Value to string.
  std::string CLtpJsonTools::JsonValueToString(const rapidjson::Value& json)
  {
    //printf("============ rapidjson::Value Created============ \n");
    // Create Writer to StringBuffer.
    rapidjson::StringBuffer strbuf1;
    rapidjson::Writer<rapidjson::StringBuffer> writer1(strbuf1);
    json.Accept(writer1); // Accept() traverses the DOM and generates Handler events.
    //printf("Condense:\n%s\n", strbuf1.GetString());

    rapidjson::StringBuffer strbuf2;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer2(strbuf2);
    json.Accept(writer2); // Accept() traverses the DOM and generates Handler events.
    //printf("Pretty:\n%s\n", strbuf2.GetString());
    //printf("============ rapidjson::Value Created============ \n");

    return strbuf1.GetString();
  }

  //rapidjson::Value can be thought of a variant type, representing a JSON value which can stores either a null value, false, true, number, string, array or object.
  void CLtpJsonTools::TypesToJsonValue(std::vector<TLocationType>& locationTypes, rapidjson::Document& document, rapidjson::Value& json)  // REQ for Ltp_methods_getUserData.
  {
  #if 1
  /*
    {
        <message-header>,
        "parameters": [
            [
                "DESTINATION",
                "LAST_KNOWN_CAR_POSITION"
            ]
        ]
    }
    */

    // For REQ format: ("parameters" : [["DESTINATION","LAST_KNOWN_CAR_POSITION"]])

    rapidjson::Value jsonInnerArr;
    jsonInnerArr.SetArray();
    jsonInnerArr.Clear();

    // Loop through the std::vector<TLocationType>, then PushBack() each <TLocationType> to the jsonInnerArr.
    for (std::vector<TLocationType>::iterator it=locationTypes.begin(); it!=locationTypes.end(); ++it)
    {
      if (ELocationType_DESTINATION == (*it))
        jsonInnerArr.PushBack(Ltp_methods_getUserData_param_type_dest, document.GetAllocator());
      else if (ELocationType_LAST_KNOWN_CAR_POSITION == (*it))
        jsonInnerArr.PushBack(Ltp_methods_getUserData_param_type_lkcp, document.GetAllocator());
      else
      {
        printf("Unknow TLocationType!\n");
        throw CLtpRuntimeException("Unknow TLocationType!");
      }
    }

    json.PushBack(jsonInnerArr, document.GetAllocator());
  #else
    // For REQ format: ("parameters" : [0,1,...])

    // Set this value as an empty array.
    json.SetArray();
    json.Clear();

    // Loop through the std::vector<TLocationType>, then PushBack() each <TLocationType> to this JSON Array.
    for (std::vector<TLocationType>::iterator it=locationTypes.begin(); it!=locationTypes.end(); ++it)
      json.PushBack((*it), document.GetAllocator());
  #endif
    // Convert a rapidjson::Value to string.
    //printf("TypesToJsonValue by std::vector<TLocationType>: %s\n", JsonValueToString(json).c_str());
  }

  //rapidjson::Value can be thought of a variant type, representing a JSON value which can stores either a null value, false, true, number, string, array or object.
  void CLtpJsonTools::TypesToJsonValue(std::vector<CLtpTypedLocation>& typedLocations, rapidjson::Document& document, rapidjson::Value& json)  // RESP for Ltp_methods_getUserData.
  {
  #if 1
  /*
    {
       <message-header>,
        "resp_code":0,
        "parameters":[
            {
                "DESTINATION": {
                    "timestamp": "2014-02-20T17:40:21.012Z",
                    "coordinates": {
                        "latitude": 345,
                        "longitude": 789
                    }
                },
                "LAST_KNOWN_CAR_POSITION": {
                    "timestamp": "2014-02-20T17:40:21.012Z",
                    "coordinates": {
                        "latitude": 321,
                        "longitude": 654
                    }
                }
            }
        ]
    }
    */

    // For RESP format: ("parameters" : [{...},{...},...])

    // Loop through the std::vector<CLtpTypedLocation>, then PushBack() each <CLtpTypedLocation> to this JSON Array.
    rapidjson::Value JObjFinal(rapidjson::kObjectType);
    for (std::vector<CLtpTypedLocation>::iterator it=typedLocations.begin(); it!=typedLocations.end(); ++it)
    {
      rapidjson::Value JObjType(rapidjson::kObjectType);
      rapidjson::Value JObjCoor(rapidjson::kObjectType);

      JObjCoor.AddMember(Ltp_methods_getUserData_param_latitude, it->getNavData().getCoordinates().getLatitude(), document.GetAllocator());
      JObjCoor.AddMember(Ltp_methods_getUserData_param_longitude, it->getNavData().getCoordinates().getLongitude(), document.GetAllocator());

      JObjType.AddMember(Ltp_methods_getUserData_param_timestamp, it->getNavData().getTimestamp().c_str(), document.GetAllocator());
      JObjType.AddMember(Ltp_methods_getUserData_param_coordinates, JObjCoor, document.GetAllocator());

      if (ELocationType_DESTINATION == it->getNavDataType())
        JObjFinal.AddMember(Ltp_methods_getUserData_param_type_dest, JObjType, document.GetAllocator());
      else if (ELocationType_LAST_KNOWN_CAR_POSITION == it->getNavDataType())
        JObjFinal.AddMember(Ltp_methods_getUserData_param_type_lkcp, JObjType, document.GetAllocator());
    }
    json.PushBack(JObjFinal, document.GetAllocator());
  #else
    // For RESP format: ("parameters" : [{...},{...},...])

    // Set this value as an empty array.
    json.SetArray();
    json.Clear();

    // Loop through the std::vector<CLtpTypedLocation>, then PushBack() each <CLtpTypedLocation> to this JSON Array.
    for (std::vector<CLtpTypedLocation>::iterator it=typedLocations.begin(); it!=typedLocations.end(); ++it)
    {
      rapidjson::Value JObjtmp(rapidjson::kObjectType);
      JObjtmp.AddMember(Ltp_methods_getUserData_param_location_type, it->getNavDataType(), document.GetAllocator());
      JObjtmp.AddMember(Ltp_methods_getUserData_param_timestamp, it->getNavData().getTimestamp().c_str(), document.GetAllocator());//it->getNavData().getTimestamp().c_str()
      JObjtmp.AddMember(Ltp_methods_getUserData_param_latitude, it->getNavData().getCoordinates().getLatitude(), document.GetAllocator());
      JObjtmp.AddMember(Ltp_methods_getUserData_param_longitude, it->getNavData().getCoordinates().getLongitude(), document.GetAllocator());
      json.PushBack(JObjtmp, document.GetAllocator());
    }

    //DBG.
    //for (rapidjson::Value::ConstValueIterator itr = json.Begin(); itr != json.End(); ++itr)
    //{
    //  CLtpJsonTools::QueringObjectMembersAndTypes(*itr);
    //}
  #endif
    //DBG.
    //printf("TypesToJsonValue by std::vector<CLtpTypedLocation>: %s\n", CLtpJsonTools::JsonValueToString(json).c_str());
  }

  void CLtpCommonTools::ReserveStringCapacity(std::string& aString, size_t aSize, size_t aUnitSize)
  {  
    const size_t allocSize = (aSize + aUnitSize - 1) & ~(aUnitSize - 1); // pack with aUnitSize  
    aString.resize(allocSize);
  }

  ssize_t CLtpCommonTools::write_all(int fd, const void* buffer, size_t count)
  {
    size_t left_to_write = count;

    while (left_to_write > 0) 
    {
      ssize_t written = write(fd, buffer, count);
      
      if (written == -1)
      {
        /* An error occurred; bail. */
        perror("Socket write_all error");
        return -1;
      }
      else
      {
        /* Keep count of how much more we need to write. */
        left_to_write -= written;
      }
    }
    
    /* We should have written no more than COUNT bytes! */
    assert(left_to_write == 0);
    
    /* The number of bytes written is exactly COUNT. */
    return count;
  }

  ssize_t CLtpCommonTools::readLine(int aFd, void *aBuffer, size_t aMaxLength)
  {
    ssize_t readCnt;  // Number of bytes fetched in this read() 
    size_t readTotal; // Total bytes alread readed so far 
    
    char *pBuf; // A pointer to bytes in aBuffer
    char ch;    // Store one char from read()

    // Preconditions.
    if (aMaxLength <= 0 || aBuffer == NULL) 
    {
      errno = EINVAL;
      return -1;
    }
    
    pBuf = (char*)aBuffer;
    readTotal = 0;

    // Read each byte with read() from aFd, tell if we get one line.
    while (1)
    {
      // Read just one byte in one loop.
      readCnt = read(aFd, &ch, 1);

      if (readCnt == -1) // read failed.
      {
        if (errno == EINTR) // failed because interrupted, just restart read() 
          continue;
        else
          return -1; // read() failed
      } 
      else if (readCnt == 0) // EOF 
      { 
        if (readTotal == 0) // No bytes read; return 0 
          return 0;
        else // Some bytes read; add '\0' 
          break;
      } 
      else // One byte readed.
      { 
        // 'readCnt' must be 1 if we get here 
        if (readTotal < aMaxLength - 1) 
        { 
          // Discard > (aMaxLength - 1) bytes 
          readTotal++;
          *pBuf++ = ch;
        }
        
        if (ch == '\n')
          break;
      }
    }
    *pBuf = '\0';
    
    return readTotal;
  }
  
  void CLtpCommonTools::readOneLineBufInit(int aFd, struct TReadOneLineBuf *aRlBufRead)
  {
    aRlBufRead->fd = aFd;
    aRlBufRead->len = 0;
    aRlBufRead->next = 0;
  }

  ssize_t CLtpCommonTools::readOneLineBuf(struct TReadOneLineBuf *aRlBufRead, char *aBuffer, size_t aMaxLength)
  {
    size_t fetched_cnt; // Number of bytes fetched to aBuffer[]
    char ch;            // Store one char from read()

    // Preconditions.
    if (aMaxLength <= 0 || aBuffer == NULL) 
    {
      errno = EINVAL;
      return -1;
    }
    fetched_cnt = 0;

    // Fetch characters from aRlBufRead->buf[], up to the next new line. 
    while (1)
    {
      // If there are insufficient characters in 'tlbuf', then obtain
      // further input from the associated file descriptor. 
      if (aRlBufRead->next >= aRlBufRead->len) 
      {
        // read() from fd to buf[RL_MAX_BUF]
        aRlBufRead->len = read(aRlBufRead->fd, aRlBufRead->buf, RL_MAX_BUF);
        if (aRlBufRead->len == -1)
        {
          return -1;
        }

        if (aRlBufRead->len == 0) // End of file 
          break;

        aRlBufRead->next = 0;
      }

      // Check each bytes in buf[]
      ch = aRlBufRead->buf[aRlBufRead->next];
      aRlBufRead->next++;

      // Copy this byte to aBuffer[]
      if (fetched_cnt < aMaxLength)
        aBuffer[fetched_cnt++] = ch;

      // Found a new line, break and return fetched_cnt.
      if (ch == '\n')
        break;
    }

    return fetched_cnt;
  }

  void CLtpCommonTools::printBufHex(char *aBuffer, int aPrintSize)
  {
    printf("==================Buffer HEX :==================\n");
    int offset = 0;
    int bytes_in_a_line = 8;
    int buffer_print_size = aPrintSize;
    while (offset < buffer_print_size)
    {
      printf("0x%06x : ", offset);
      for (int i = offset; i < (offset+bytes_in_a_line); ++i)
      {
        printf("%02x ", aBuffer[i]);
      }
      printf ("\n");
      
      offset += bytes_in_a_line;
    }
    printf("================================================\n");
  }
  
  CLtpTimer::CLtpTimer() : 
     timerid(0)
    ,iSignalHandler(NULL)
  {
    printf("CLtpTimer created!\n");
  }
  
  CLtpTimer::~CLtpTimer()
  {
    printf("CLtpTimer deleted!\n");
  }
  
  void CLtpTimer::print_siginfo(siginfo_t* si)
  {
    timer_t *tidp;
    int ovr;
    
    tidp = (timer_t *)si->si_value.sival_ptr;
    
    printf("    sival_ptr = %p; ", si->si_value.sival_ptr);
    printf("    *sival_ptr = 0x%lx\n", (long) *tidp);
    
    ovr = timer_getoverrun(*tidp);
    if (ovr == -1)
    {
      perror("timer_getoverrun err");
    }
    else
      printf("    overrun count = %d\n", ovr);
  }

  void CLtpTimer::POSIX_interval_timer_expire_handler(int sig, siginfo_t* si, void* uc)
  {
    /* UNSAFE: This handler uses non-async-signal-safe functions printf() */
    printf("CLtpTimer::POSIX_interval_timer_expire_handler()\n");
    printf("[%s] Got signal %d\n", CLtpTimestampedLocation::GetLocalTimeStamp().c_str(), sig);
    CLtpTimer::print_siginfo(si);
  }

  void CLtpTimer::fn_timer_handler_GoCountDownSleeper(int sig, siginfo_t* si, void* uc)
  {
    // Do nothing.
    //printf("fn_timer_handler_GoCountDownSleeper() timeout\n");
  }

  int CLtpTimer::TimerCreate(FN_LTP_TIMER_SIGNAL_HANDLER aSignalHandler, int aSignalNum)
  {
    struct sigaction sa;
    struct sigevent sev;

    if ( (aSignalNum < SIGRTMIN) || (aSignalNum > SIGRTMAX) )
    {
      printf("TimerCreate error! Wrong Signal range!!");
      return 0;
    }
    
    printf("_POSIX_C_SOURCE:%ld\n", _POSIX_C_SOURCE);

    /* Establish handler for timer signal */
    printf("Establishing handler for signal %d\n", aSignalNum);
    memset(&sa, 0, sizeof (sa));
    sa.sa_flags = SA_SIGINFO;
    if (NULL == aSignalHandler)
      sa.sa_sigaction = &CLtpTimer::POSIX_interval_timer_expire_handler;
    else
      sa.sa_sigaction = aSignalHandler;
    if (sigaction(aSignalNum, &sa, NULL) < 0)
    {
      perror("sigaction err");
      return 0;
    }

    /* Create the timer */
    sev.sigev_notify = SIGEV_SIGNAL; /* Notify via signal */
    sev.sigev_signo = aSignalNum; /* Notify using this signal */
    sev.sigev_value.sival_ptr = &timerid;
    if (timer_create(LTP_TIMER_CLOCKID, &sev, &timerid) < 0)
    {
      perror("timer_create err");
      return 0;
    }
    //printf("Timer[%ld] Created\n", (long)timerid);

    return 1;
  }
  
  int CLtpTimer::TimerStart(time_t aValue_tv_sec, long aValue_tv_nsec, time_t aInterval_tv_sec, long aInterval_tv_nsec)
  {
    struct itimerspec its;
    
    /* Start the timer */
    its.it_value.tv_sec = aValue_tv_sec;
    its.it_value.tv_nsec = aValue_tv_nsec;
    its.it_interval.tv_sec = aInterval_tv_sec;
    its.it_interval.tv_nsec = aInterval_tv_nsec;
    if (timer_settime(timerid, 0, &its, NULL) < 0)
    {
      perror("timer_settime err");
      return 0;
    }
    //printf("Timer[%ld] Started [%ld,%ld,%ld,%ld] !\n", (long)timerid, aValue_tv_sec, aValue_tv_nsec, aInterval_tv_sec, aInterval_tv_nsec);
    
    return 1;
  }

  int CLtpTimer::TimerStop()
  {
    struct itimerspec its;
    
    /* Stop the timer */
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 0;
    its.it_interval.tv_nsec = 0;
    if (timer_settime(timerid, 0, &its, NULL) < 0)
    {
      perror("timer_settime err");
      return 0;
    }
    //printf("Timer[%ld] Stop !\n", (long)timerid);
    
    return 1;
  }
  
  int CLtpTimer::TimerDelete()
  {
    if (timer_delete(timerid) < 0)
    {
      perror("timer_delete err");
      return 0;
    }
    //printf("Timer[%ld] Deleted\n", (long)timerid);
    
    return 1;
  }

  int CLtpTimer::GoNanoSleeper(int aSeconds)
  {
    printf("CLtpTimer::GoNanoSleeper(%d)\n", aSeconds);
    
    int result = 0;
    struct timespec tp, res, tp_tmp;

    if (clock_getres(LTP_TIMER_CLOCKID, &res) == -1)
    {
      perror("clock_getres");
    }
    //printf("res.tv_sec:%ld\n", res.tv_sec);
    //printf("res.tv_nsec:%ld\n", res.tv_nsec);

    /* Retrieve current value of LTP_TIMER_CLOCKID clock */
    if (clock_gettime(LTP_TIMER_CLOCKID, &tp) == -1)
    {
      perror("clock_gettime");
      return 0;
    }
    //printf("Current clock_gettime:\n");
    //printf("\ttp.tv_sec:%ld\n", tp.tv_sec);
    //printf("\ttp.tv_nsec:%ld\n", tp.tv_nsec);

    tp.tv_sec += aSeconds; /* Sleep for aSeconds seconds from now */
    //printf("Set expect clock_nanosleep till:\n");
    //printf("\ttp.tv_sec:%ld\n", tp.tv_sec);
    //printf("\ttp.tv_nsec:%ld\n", tp.tv_nsec);

    while (1)
    {
      //printf("Keep clock_nanosleep... \n");
      result = clock_nanosleep(LTP_TIMER_CLOCKID, TIMER_ABSTIME, &tp, NULL);
      if (result != 0) 
      {
        if (result == EINTR)
        {
          printf("clock_nanosleep Interrupted by signal handler at time:\n");

          /* Retrieve current value of LTP_TIMER_CLOCKID clock */
          if (clock_gettime(LTP_TIMER_CLOCKID, &tp_tmp) == -1)
          {
            perror("clock_gettime");
          }
          printf("\ttp_tmp.tv_sec:%ld\n", tp_tmp.tv_sec);
          printf("\ttp_tmp.tv_nsec:%ld\n", tp_tmp.tv_nsec);
        }
        else
        {
          //printf("clock_nanosleep result:%d", result);
          return 0;
        }
      }
      else
      {
        /* Retrieve current value of LTP_TIMER_CLOCKID clock */
        if (clock_gettime(LTP_TIMER_CLOCKID, &tp_tmp) == -1)
        {
          perror("clock_gettime");
          return 0;
        }
        //printf("Sleep Done!!! Current clock_gettime:\n");
        //printf("\ttp_tmp.tv_sec:%ld\n", tp_tmp.tv_sec);
        //printf("\ttp_tmp.tv_nsec:%ld\n", tp_tmp.tv_nsec);
        break;
      }
    }

    return 1;
  }

  int CLtpTimer::GoCountDownSleeper(int aSeconds)
  {
    printf("GoCountDownSleeper for %d seconds.\n", aSeconds);
    int cnt;

    cnt = 0;
    while (cnt < aSeconds)
    {
      // start a 1 second timer.
      GoNanoSleeper(1);
      
      printf("%d\n", aSeconds - cnt);
      cnt++;
    }

    return 1;
  }

  //--------------------------------
  CPThread::CPThread() : 
    iThreadId(0)
    ,iThreadRunning(0)
    ,iThreadDetached(0) 
  {
  }

  CPThread::~CPThread()
  {
    if (iThreadRunning == 1 && iThreadDetached == 0) 
    {
      pthread_detach(iThreadId);
    }
    
    if (iThreadRunning == 1) 
    {
      pthread_cancel(iThreadId);
    }
  }

  void* CPThread::runThread(void* arg)
  {
    return ((CPThread*)arg)->run();
  }
  
  int CPThread::start()
  {
    int result = pthread_create(&iThreadId, NULL, runThread, this);
    
    if (result == 0) 
    {
      iThreadRunning = 1;
    }
    
    return result;
  }

  int CPThread::join()
  {
    int result = -1;
    
    if (iThreadRunning == 1) 
    {
      result = pthread_join(iThreadId, NULL);
      
      if (result == 0) 
      {
        iThreadDetached = 0;
      }
    }
    
    return result;
  }

  int CPThread::detach()
  {
    int result = -1;
    
    if (iThreadRunning == 1 && iThreadDetached == 0) 
    {
      result = pthread_detach(iThreadId);
      
      if (result == 0) 
      {
        iThreadDetached = 1;
      }
    }
    
    return result;
  }

  pthread_t CPThread::self() 
  {
    return iThreadId;
  }
  //--------------------------------

  //--------------------------------
  CPthreadMutex::CPthreadMutex() 
  {
    if (pthread_mutexattr_init(&iMutexAttr) != 0)
    {
      perror("pthread_mutexattr_init");
      //exit(EXIT_FAILURE);
      return;
    }

    if (pthread_mutexattr_settype(&iMutexAttr, PTHREAD_MUTEX_ERRORCHECK) != 0)
    {
      perror("pthread_mutexattr_settype");
      //exit(EXIT_FAILURE);
      return;
    }
    
    if (pthread_mutex_init(&iMutex, &iMutexAttr) != 0)
    {
      perror("pthread_mutex_init");
      //exit(EXIT_FAILURE);
      return;
    }
    
    if (pthread_mutexattr_destroy(&iMutexAttr) != 0) // No longer needed
    {
      perror("pthread_mutexattr_destroy");
      //exit(EXIT_FAILURE);
      return;
    }
  }

  CPthreadMutex::~CPthreadMutex() 
  {
    if (pthread_mutex_destroy(&iMutex) != 0)
    {
      perror("pthread_mutexattr_settype");
      //exit(EXIT_FAILURE);
      return;
    }
  }

  int CPthreadMutex::lock() 
  {
    return pthread_mutex_lock(&iMutex);
  }
  
  int CPthreadMutex::trylock() 
  {
    return pthread_mutex_trylock(&iMutex); 
  }
  
  int CPthreadMutex::unlock() 
  {
    return pthread_mutex_unlock(&iMutex); 
  }
  //--------------------------------

  //--------------------------------
  CPthreadCondVar::CPthreadCondVar(CPthreadMutex& mutex) : 
    iLock(mutex) 
  {
    if (pthread_cond_init(&iCond, NULL) != 0)
    {
      perror("pthread_cond_init");
      //exit(EXIT_FAILURE);
      return;
    }
  }
  
  CPthreadCondVar::~CPthreadCondVar() 
  {
    if (pthread_cond_destroy(&iCond) != 0)
    {
      perror("pthread_cond_destroy");
      //exit(EXIT_FAILURE);
      return;
    }
  }

  int CPthreadCondVar::wait()
  {
    return pthread_cond_wait(&iCond, &(iLock.iMutex)); 
  }
  
  int CPthreadCondVar::signal() 
  { 
    return pthread_cond_signal(&iCond); 
  } 
  
  int CPthreadCondVar::broadcast() 
  { 
    return pthread_cond_broadcast(&iCond); 
  } 
  //--------------------------------
  
} //namespace NBsc
