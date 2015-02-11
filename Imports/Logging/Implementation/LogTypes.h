// LogTypes.h
//
// Description:
// All kind of types used by the logging component.
//

#ifndef _LOGTYPES_
#define _LOGTYPES_

// Includes
#include <stdint.h>

// Namespace definition
namespace logging
{
  //! The log category type definition
  typedef int32_t TLogCategory;

  //! The log compoment type definition
  typedef int32_t TLogComponent;

  //! The log compoment type definition
  typedef uint32_t TLogProtocol;

  //! Standard component identifiers (0x100, 0x200 and onwards are
  //! reserverd for product components).
  static const int32_t KNrOfComponents            = 256;
  static const TLogComponent AllComponents        = 0xFFFFFFFF;

  // Default log components
  static const char* const KLogMain            = "Main";
  static const char* const KLogConfigurationFw = "ConfigurationFw";
  static const char* const KLogConnectionFw    = "ConnectionFw";
  static const char* const KLogFeatureFw       = "FeatureFw";
  static const char* const KLogUtilitiesFw     = "UtilitiesFw";

  /*
  // Standard log components
  static const TLogComponent OSAL                 = 20;
  static const TLogComponent UART                 = 21;
  static const TLogComponent GUI                  = 22;
  static const TLogComponent SWLInfra             = 23;
  static const TLogComponent SPI                  = 24;
  */

  // Namespace definition
  namespace categories
  {
    //! The supported log categories
    static const int32_t KNrOfCategories        = 10;
    static const TLogCategory KAllCategories    = 0xFFFFFFFF;
    static const TLogCategory KError            = 0x00000001;
    static const TLogCategory KWarning          = 0x00000002;
    static const TLogCategory KInfo             = 0x00000004;
    static const TLogCategory KInfo2            = 0x00000008;
    static const TLogCategory KInterface        = 0x00000010;
    static const TLogCategory KEvent            = 0x00000020;
    static const TLogCategory KState            = 0x00000040;
    static const TLogCategory KAlways           = 0x00000080;
    static const TLogCategory KTA               = 0x00000100;
    static const TLogCategory KAssert           = 0x00000200;
  } // namespace categories

  // Namespace definition
  namespace protocols
  {
    //! The supported log protocols
    static const int32_t KNrOfProtocols         = 9;
    static const TLogProtocol Proto_TWUTL       = 0x00000001;
    static const TLogProtocol Proto_PRISM       = 0x00000002;
    static const TLogProtocol Proto_REFLECTION  = 0x00000004;
    static const TLogProtocol Proto_FOCUS       = 0x00000010;
    static const TLogProtocol Proto_TTMP        = 0x00000020;
    static const TLogProtocol Proto_PACKET      = 0x00000040;
    static const TLogProtocol Proto_BLUETOOTH   = 0x00000100;
    static const TLogProtocol Proto_GPS         = 0x00000200;
    static const TLogProtocol Proto_RAW         = 0x00000400;
  } // namespace protocols

  namespace internals
  {
    //! Contains the used seperator between strings
    static const char* const KSeperator = "  ";

    //! Contains the new line character
    static const char* const KNewline = "\n";

    //! The log control file syntax
    static const char* const KConfigFile = "logsettings.dat";

    #ifdef ANDROID
    static const char* const KConfigFilePath  = "/data/";
    static const char* const KConfigFilePath2 = "";
    #elif LINUX_TARGET
    static const char* const KConfigFilePath  = "/mnt/sdcard/";
    static const char* const KConfigFilePath2 = "/mnt/movinand/";
    #else
    static const char* const KConfigFilePath  = "";
    static const char* const KConfigFilePath2 = "";
    #endif  // LINUX_TARGET

    static const uint32_t KMaxPathLength                       = 255;
    static const uint32_t KMaxLineSize                         = 512;
    static const char KControlFileComment                      = '#';
    static const char KControlFileAstrics                      = '*';
    static const char KControlFileMinus                        = '-';
    static const char KControlFileProcessSectionBegin          = '[';
    static const char KControlFileProcessSectionEnd            = ']';
    static const char KControlFileProcessComponentBegin        = '<';
    static const char KControlFileProcessComponentEnd          = '>';
    static const char* const KControlFileProcessOutputFileName = "OutputFileName";
    static const char* const KControlFileComponentCategory     = "Category";
    static const char* const KControlFileComponentOutputMethod = "Output";
    static const char* const KControlFileOutputMethodScreen    = "Screen";
    static const char* const KControlFileOutputMethodSyslog    = "Syslog";
    static const char* const KControlFileOutputMethodFile      = "File";
    #ifdef DIAGNOSTIC_LOG_TRACE
    static const char* const KControlFileOutputMethodDlt       = "Dlt";
    #endif //DIAGNOSTIC_LOG_TRACE
    static const char* const KControlFileSeperatorComponent    = "=";
    static const char* const KControlFileSeperatorCategory     = "|";

    enum TLogType
    {
      ELogTypeScreen  = 0x01,
      ELogTypeFile    = 0x02,
      ELogTypeSyslog  = 0x04,
    #ifdef DIAGNOSTIC_LOG_TRACE
      ELogTypeDlt     = 0x08,
    #endif //DIAGNOSTIC_LOG_TRACE
      ELogTypeUnitTest= 0x10
    };
  } // namespace internal
} // namespace logging

#endif  // _LOGTYPES_
