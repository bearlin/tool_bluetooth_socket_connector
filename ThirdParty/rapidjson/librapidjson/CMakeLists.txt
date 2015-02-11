project(rapidjson)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(ThirdParty_rapidjson_librapidjson_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_rapidjson_librapidjson_ROOT directory")
  #message(STATUS "*** ThirdParty_rapidjson_librapidjson_ROOT = ${ThirdParty_rapidjson_librapidjson_ROOT}")
  
# Some adjustable directory names.
set(INTERFACE_DIR "Interface")
set(RAPIDJSON_INTERNAL_DIR "internal" CACHE STRING "Directory for the internal files")
  #message(STATUS "*** using variable 'INTERFACE_DIR '=${INTERFACE_DIR}")
  #message(STATUS "*** using variable 'RAPIDJSON_INTERNAL_DIR '=${RAPIDJSON_INTERNAL_DIR}")

# Definition of the rapidjson include files
set(RAPIDJSON_INCLUDES
  ${CMAKE_CURRENT_LIST_DIR}/${INTERFACE_DIR}
	CACHE STRING "rapidjson API INCLUDES path")
  #message(STATUS "*** using variable 'RAPIDJSON_INCLUDES'=${RAPIDJSON_INCLUDES}")

set(ThirdParty_rapidjson_librapidjson_API_INCLUDES
    ${RAPIDJSON_INCLUDES}
    CACHE STRING "ThirdParty_rapidjson_librapidjson_API_INCLUDES path")
  #message(STATUS "*** using variable 'ThirdParty_rapidjson_librapidjson_API_INCLUDES'=${ThirdParty_rapidjson_librapidjson_API_INCLUDES}")

#Include path
include_directories( ${ThirdParty_rapidjson_librapidjson_API_INCLUDES} )

# Header files
file_glob( RAPIDJSON_HEADER_FILES ${RAPIDJSON_INCLUDES}/*.h)
# Source files
file_glob( RAPIDJSON_INTERNAL_CPP_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.cpp)
  #message(STATUS "*** using variable 'RAPIDJSON_HEADER_FILES'=${RAPIDJSON_HEADER_FILES}")
  #message(STATUS "*** using variable 'RAPIDJSON_INTERNAL_CPP_FILES '=${RAPIDJSON_INTERNAL_CPP_FILES}")

add_library(${PROJECT_NAME} STATIC ${RAPIDJSON_INTERNAL_CPP_FILES} ${RAPIDJSON_HEADER_FILES} )

disable_warnings_as_errors_on_target(${PROJECT_NAME})

