//
// This file contain common datas for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPCOMMON_H_
#define _LTPCOMMON_H_

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h" // for stringify JSON
#include "rapidjson/stringbuffer.h" // https://code.google.com/p/rapidjson/issues/detail?id=34
#include "rapidjson/prettywriter.h"

// For POSIX timer.
#include <signal.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  // Include global variables from LtpCommon.cpp.
  //-------------------------------------------------
  // Message types list:
  extern const char* Ltp_msg_types_req;
  extern const char* Ltp_msg_types_resp;

  // Message key strings:
  extern const char* Ltp_ptl_ver;
  extern const char* Ltp_major;
  extern const char* Ltp_minor;
  extern const char* Ltp_msg_type;
  extern const char* Ltp_msg_id;
  extern const char* Ltp_method;
  extern const char* Ltp_resp_code;
  extern const char* Ltp_parameters;

  // Methods list:
  extern const char* Ltp_methods_TestFunc;
  extern const char* Ltp_methods_getUserData;

  // Method "getNavData" parameters:
  extern const char* Ltp_methods_getUserData_param_location_type;
  extern const char* Ltp_methods_getUserData_param_timestamp;
  extern const char* Ltp_methods_getUserData_param_latitude;
  extern const char* Ltp_methods_getUserData_param_longitude;
  extern const char* Ltp_methods_getUserData_param_coordinates;

  // Method "getNavData" type strings:
  extern const char* Ltp_methods_getUserData_param_type_dest;
  extern const char* Ltp_methods_getUserData_param_type_lkcp;

  // rapidjson types.
  extern const char* kTypeNames[];

  extern const int KMessageBufUnitSize;
  //-------------------------------------------------

  enum TConnectionStatus
  {
    EConnectionStatusCONNECTED   = 0,
    EConnectionStatusNOT_CONNECTED
  };
    
  enum TLocationType 
  {
      ELocationType_DESTINATION = 0,
      ELocationType_LAST_KNOWN_CAR_POSITION
  };

  enum TSocketType 
  {
      ESocketType_IP = 0,
      ESocketType_BT
  };

  /**
   * Coordinates of a location. Coordinate's latitude and longitude are
   * represented in microdegrees (decimal degrees * 10^6). A negative
   * longitude means West (of Greenwich) and a negative latitude means South
   * (of the equator).
   */
  class CLtpWGS84Coordinates 
  {
  private:
    int latitude;
    int longitude;
      
  public:    
    int getLatitude() const
    {
      return latitude;
    }

    void setLatitude(int latitude) 
    {
      this->latitude = latitude;
    }

    int getLongitude() const
    {
      return longitude;
    }

    void setLongitude(int longitude) 
    {
      this->longitude = longitude;
    }
  };

  class CLtpTimestampedLocation 
  {
  private:
    // Timestamps are presented as strings using a restricted ISO8601 format: 
    // YYYY-MM-DDThh:mm:ss.SSSZ ('T' and 'Z' are literal.). 
    // EX: "1997-07-16T19:20:30.450+0100".
    // EX: "2014-02-19T00:01:07.000+0800"
    // Check http://www.w3.org/TR/NOTE-datetime, http://www.cl.cam.ac.uk/~mgk25/iso-time.html
    // Check http://www.cplusplus.com/reference/ctime/strftime/ about how to create this timestamp format.
    std::string timestamp;
    CLtpWGS84Coordinates coordinates;
    
  public:
    std::string getTimestamp() const 
    {
      return timestamp;
    }

    void setTimestamp(std::string timestamp) 
    {
      this->timestamp = timestamp;
    }

    CLtpWGS84Coordinates getCoordinates() const
    {
      return coordinates;
    }

    void setCoordinates(CLtpWGS84Coordinates coordinates) 
    {
      this->coordinates = coordinates;
    }

    static std::string GetLocalTimeStamp();
  };

  class CLtpTypedLocation 
  {
  private:
    TLocationType location_type;
    CLtpTimestampedLocation timestamped_location;

  public:
    CLtpTypedLocation() : location_type(ELocationType_DESTINATION) {}
    
    TLocationType getNavDataType() const
    {
      return location_type;
    }

    void setNavDataType(TLocationType locationType) 
    {
      this->location_type = locationType;
    }

    CLtpTimestampedLocation getNavData() const
    {
      return timestamped_location;
    }

    void setNavData(CLtpTimestampedLocation navData) 
    {
      this->timestamped_location = navData;
    }
  };

  class CLtpJsonTools 
  {
  private:
    
  public:
    static int QueringObjectMembersAndTypes(const rapidjson::Value& json);
    
    static std::string JsonValueToString(const rapidjson::Value& json);
    static void TypesToJsonValue(std::vector<TLocationType>& locationTypes, rapidjson::Document& document, rapidjson::Value& json);  // REQ for Ltp_methods_getUserData.
    static void TypesToJsonValue(std::vector<CLtpTypedLocation>& typedLocations, rapidjson::Document& document, rapidjson::Value& json);  // RESP for Ltp_methods_getUserData.
  };

  // For CLtpCommonTools::readOneLineBufInit()
  //-------------------------------------------------
  #define RL_MAX_BUF (10)         // Size of buf[] and each read().
  struct TReadOneLineBuf {
      int     fd;                 // File descriptor from which to read 
      char    buf[RL_MAX_BUF];    // Current buffer from file 
      int     next;               // Index of next unread character in 'buf' 
      ssize_t len;                // Number of characters in 'buf' after read().
  };
  //-------------------------------------------------
  
  class CLtpCommonTools 
  {
  private:
    
  public:
    static void ReserveStringCapacity(std::string& aString, size_t aSize, size_t aUnitSize); 
    
    //! Write all of COUNT bytes from BUFFER to file descriptor FD.  
    //! \param[in]    fd      The file descriptor.
    //! \param[in]    buffer  The buffer pointer to the data.
    //! \param[in]    count   The number of bytes written.
    //! \return -1 on error, or the number of bytes written. 
    static ssize_t write_all(int fd, const void* buffer, size_t count);

    //! Read characters from 'aFd' until a newline is encountered.
    //! If a newline character is not encountered in the first (n - 1) bytes, then the excess characters are discarded.
    //! \param[in]    aFd         The file descriptor.
    //! \param[out]   aBuffer     The returned string placed in 'aBuffer' is null-terminated and includes the newline character if it was read in the first (n - 1) bytes.
    //! \param[in]    aMaxLength  The maximum number of bytes read.
    //! \return -1 on error, or the number of bytes placed in aBuffer (which includes the newline character if encountered, but excludes the terminating null byte).
    static ssize_t readLine(int aFd, void *aBuffer, size_t aMaxLength);
    
    //! Initialize a TReadOneLineBuf structure
    //! \param[in]    aFd         The file descriptor.
    //! \param[in]    aRlBufRead  A pointer to (struct TReadOneLineBuf) which will store the read line buffer information.
    static void readOneLineBufInit(int aFd, struct TReadOneLineBuf *aRlBufRead);

    //! Return a line of input from the buffer 'aRlBufRead->buf[]', placing the characters in 'aBuffer'.
    //! \param[in]    aRlBufRead  The pointer to a pre-initialized (struct TReadOneLineBuf).
    //! \param[out]   aBuffer     The returned string placed in 'aBuffer'.
    //! \param[in]    aMaxLength  The size of 'aBuffer'. If the line of input is larger than this, then the excess characters are discarded.
    //! \return -1 on error, or the number of bytes placed in aBuffer (which includes the newline character if encountered, but excludes the terminating null byte).
    static ssize_t readOneLineBuf(struct TReadOneLineBuf *aRlBufRead, char *aBuffer, size_t aMaxLength);
    
    //http://jerrypeng.me/2013/08/mythical-40ms-delay-and-tcp-nodelay/
    //static void _set_tcp_nodelay(int fd) 
    //{
    //  int enable = 1;
    //  setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (void*)&enable, sizeof(enable));
    //}

    static void printBufHex(char *aBuffer, int aPrintSize);
  };

  // For CLtpTimer.
  //-------------------------------------------------
  #define LTP_TIMER_CLOCKID CLOCK_REALTIME
  #define LTP_TIMER_SIG SIGRTMIN /* Our timer notification signal */
  #define SIG_TIMER1 (LTP_TIMER_SIG)      // signal number for timer object 1.
  #define SIG_TIMER2 (LTP_TIMER_SIG + 1)    // signal number for timer object 2.
  typedef void (*FN_LTP_TIMER_SIGNAL_HANDLER)(int sig, siginfo_t* si, void* uc);
  //-------------------------------------------------
  
  class CLtpTimer 
  {
  private:
    timer_t timerid;
    FN_LTP_TIMER_SIGNAL_HANDLER iSignalHandler;
    
  public:    
    CLtpTimer();
    virtual ~CLtpTimer();
    
    static void print_siginfo(siginfo_t* si);
    static void POSIX_interval_timer_expire_handler(int sig, siginfo_t* si, void* uc);
    static void fn_timer_handler_GoCountDownSleeper(int sig, siginfo_t* si, void* uc);

    int TimerCreate(FN_LTP_TIMER_SIGNAL_HANDLER aSignalHandler, int aSignalNum);
    int TimerStart(time_t aValue_tv_sec, long aValue_tv_nsec, time_t aInterval_tv_sec, long aInterval_tv_nsec);
    int TimerStop();
    int TimerDelete();

    // Some handy timer APIs.
    //
    // Use clock_nanosleep(LTP_TIMER_CLOCKID, TIMER_ABSTIME, &tp, NULL) to sleep for a while,
    // will keep sleeping till the specified time if sleeper interruptted by signals. 
    static int GoNanoSleeper(int aSeconds);
    static int GoCountDownSleeper(int aSeconds);
  };

  class CPThread
  {
  private:
    pthread_t  iThreadId;
    int        iThreadRunning;
    int        iThreadDetached;
    
  public:
    CPThread();
    virtual ~CPThread();

    // All derived objects share this static function and input their 'this' pointer to 'arg'.
    // Derived objects' run() will be call in this function.
    static void* runThread(void* arg);

    // Create and start thread runThread(this).
    int start();

    // Join derived object's thread.
    int join();

    // Detach derived object's thread.
    int detach();

    // Get derived object's thread ID.
    pthread_t self();

    // Actual thread content should be implemented here by derived class.
    virtual void* run() = 0;
  };
  
  class CPthreadMutex
  {
  private:
    friend class CPthreadCondVar;
    pthread_mutex_t  iMutex;
    pthread_mutexattr_t iMutexAttr;

  public:
    // Initialize mutex to PTHREAD_MUTEX_ERRORCHECK.
    CPthreadMutex();
    virtual ~CPthreadMutex();
    int lock();
    int trylock();
    int unlock();
  };

  class CPthreadCondVar
  {
  private:
    pthread_cond_t  iCond;
    CPthreadMutex&  iLock;

    // Prevent calling applications from invoking it since it makes no sense to have a CondVar object with no Mutex object.
    CPthreadCondVar();
    
  public:
    // just initialize to defaults
    CPthreadCondVar(CPthreadMutex& mutex);
    virtual ~CPthreadCondVar();
    int wait();
    int signal();
    int broadcast();
  };
  
  
} //namespace NBsc

#endif  // _LTPCOMMON_H_
