//
// This file contain Exceptions for Location Information Transfer Over IP/Bluetooth Socket.
//

#ifndef _LTPEXCEPTIONS_H_
#define _LTPEXCEPTIONS_H_

#include "LtpCommon.h"

#include <iostream>
#include <string>
#include <stdexcept>

namespace NBsc //Namespace is BluetoothSocketConnectors
{
  class CLtpBaseException
  {
  public:
    std::string baseExceptionPrefixStr;
    std::string exceptionStr;

    CLtpBaseException() : baseExceptionPrefixStr("CLtpBaseException: ")
    {
      exceptionStr = baseExceptionPrefixStr;
    }
    
    virtual const char* getExceptionPrefixStr()
    {
      return baseExceptionPrefixStr.c_str();
    }
    
    virtual const char* what() const throw()
    {
      return exceptionStr.c_str();
    }
  };
  
  class CLtpIllegalArgumentException : public CLtpBaseException
  {
  private:
    std::string message;
    std::string illegalArgumentExceptionPrefixStr;
    
  public:  
    CLtpIllegalArgumentException(const std::string msg) : 
      message(msg), 
      illegalArgumentExceptionPrefixStr("CLtpIllegalArgumentException: ")
    {
      exceptionStr = illegalArgumentExceptionPrefixStr + message;
    }

    virtual const char* getExceptionPrefixStr()
    {
      return illegalArgumentExceptionPrefixStr.c_str();
    }
    
    virtual const char* what() const throw()
    {
      return exceptionStr.c_str();
    }
  };
  
  class CLtpJsonParseException : public CLtpBaseException
  {
  private:
    std::string message;
    std::string jsonParseExceptionPrefixStr;
    
  public:  
    CLtpJsonParseException(const std::string msg) : 
      message(msg), 
      jsonParseExceptionPrefixStr("CLtpJsonParseException: ")
    {
      exceptionStr = jsonParseExceptionPrefixStr + message;
    }

    virtual const char* getExceptionPrefixStr()
    {
      return jsonParseExceptionPrefixStr.c_str();
    }
    
    virtual const char* what() const throw()
    {
      return exceptionStr.c_str();
    }
  };

  class CLtpRuntimeException : public CLtpBaseException
  {
  private:
    std::string message;
    std::string runtimeExceptionPrefixStr;
    
  public:  
    CLtpRuntimeException(const std::string msg) : 
      message(msg),
      runtimeExceptionPrefixStr("CLtpRuntimeException: ")
    {
      exceptionStr = runtimeExceptionPrefixStr + message;
    }

    virtual const char* getExceptionPrefixStr()
    {
      return runtimeExceptionPrefixStr.c_str();
    }
    
    virtual const char* what() const throw()
    {
      return exceptionStr.c_str();
    }
  };
} //namespace NBsc

#endif  // _LTPEXCEPTIONS_H_