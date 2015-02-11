// ILogging.h
//
// Description:
// Logging Main include file for the logging component
//

#ifndef _ILOGGING_
#define _ILOGGING_

// Logging component types
#include "LogTypes.h"
#include <string>
#include <map>
#include <typeinfo>

// The actual log methods
namespace logging
{
  // Logging macros
  //----------------------------------------
  //#define LOGGING_ENABLE_FILE_AND_LINE
  
  #define LOGGING_ENABLE_INTERFACE
  #define LOGGING_ENABLE_ERROR
  #define LOGGING_ENABLE_INFO
  #if defined(LOGGING_ENABLE_INTERFACE)
    #define LogInterface(aComponentId, aMsg)  LogMethod(aComponentId, aMsg)
  #else
    #define LogInterface(aComponentId, aMsg)
  #endif

  #if defined(LOGGING_ENABLE_ERROR)
    #define LogError(aComponentId, aMsg) LogInterface(aComponentId, aMsg)
  #else
    #define LogError(aComponentId, aMsg)
  #endif

  #if defined(LOGGING_ENABLE_INFO)
    #define LogInfo(aComponentId, aMsg) LogInterface(aComponentId, aMsg)
  #else
    #define LogInfo(aComponentId, aMsg)
  #endif

#ifdef LOGGING_ENABLE_FILE_AND_LINE
  #define LogMethod(aComponentId, aMsg)                                         \
  {                                                                             \
    if (0 != logging::IControl().GetComponentName(aComponentId).length())       \
    {                                                                           \
      std::cout << "__FILE__:" << __FILE__ << "(" << __LINE__ << ") : ";        \
      std::cout << logging::IControl().GetComponentName(aComponentId) << ": ";  \
      std::cout << "[__FUNCTION__: " << __FUNCTION__ << "]\t";                  \
      std::cout << aMsg;                                                        \
      std::cout << std::endl;                                                   \
    }                                                                           \
  }
#else //LOGGING_ENABLE_FILE_AND_LINE
  #define LogMethod(aComponentId, aMsg)                                         \
  {                                                                             \
    if (0 != logging::IControl().GetComponentName(aComponentId).length())       \
    {                                                                           \
      std::cout << logging::IControl().GetComponentName(aComponentId) << ": ";  \
      std::cout << "[__FUNCTION__: " << __FUNCTION__ << "]\t";                  \
      std::cout << aMsg;                                                        \
      std::cout << std::endl;                                                   \
    }                                                                           \
  }
#endif //LOGGING_ENABLE_FILE_AND_LINE
  //----------------------------------------

  //! Control the logging component
  //----------------------------------------
  class ILogControl
  {
  public:
    //! Interface destructor
    virtual ~ILogControl() {}

    //! Register a component so it can be used for parsing the config-file
    //! @param aComponentStr  The component name related to the component
    virtual void RegisterComponent(const char* aStrComponent, TLogComponent& aComponentIndex) = 0;

    virtual std::string GetComponentName(TLogComponent& aComponentIndex) = 0;
  };

  //! Interface method to get a ILogControl instance
  ILogControl& IControl();
  //----------------------------------------

  // Namespace definition
  namespace internals
  {
    //! Log component
    //----------------------------------------
    class ILogBase
    {
    public:
      //! Interface destructor
      virtual ~ILogBase() {}
    };

    //! Interface method to get a ILog instance
    ILogBase& ILog();
    //----------------------------------------

    //! Basic log class definition
    //----------------------------------------
    class CLogImpl: public ILogBase
    {
    public:
      //! @brief Retrieve the singleton instance
      static CLogImpl& Instance();

      //! @brief Retrieve the ILogControl
      ILogControl* GetLogControl();

    private:
      //! Access only via singleton method
      CLogImpl();

      //! Do not allow public destruction
      virtual ~CLogImpl();

      //! Do not allow copy construction
      CLogImpl(const CLogImpl&);

      //! Do not allow assignment
      CLogImpl& operator=(const CLogImpl&);

      ILogControl*    iLogControl;
    };
    //----------------------------------------

    //! Log control definition
    //----------------------------------------
    class CLogControlImpl: public ILogControl
    {
    public:

      //! Constructor
      CLogControlImpl();

      //! Destructor
      virtual ~CLogControlImpl();

      //! Do nothing in Logging.
      virtual void RegisterComponent(const char* aStrComponent, TLogComponent& aComponentIndex);

      virtual std::string GetComponentName(TLogComponent& aComponentIndex);
        
    private:
      //! Do not allow copy construction
      CLogControlImpl(const CLogControlImpl&);

      //! Do not allow assignment
      CLogControlImpl& operator=(const CLogControlImpl&);

      TLogComponent iNextComponentIndex;
      std::map<std::string, TLogComponent> iMapComponentNameToIndex;
      std::map<TLogComponent, std::string> iMapIndexToComponentName;
    };
    //----------------------------------------
  }
} // namespace logging

#endif  // _ILOGGING_
