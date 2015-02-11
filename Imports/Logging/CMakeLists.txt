project(Imports_Logging)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")
  
set(Imports_Logging_API_INCLUDES
    ${PROJECT_SOURCE_DIR}/Implementation
    CACHE STRING "Imports_Logging_API_INCLUDES path")
  message(STATUS "*** using variable 'Imports_Logging_API_INCLUDES'=${Imports_Logging_API_INCLUDES}")
  
include_directories(
    ${Imports_Logging_API_INCLUDES}
)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.h)
# Make a list of the source files we use.
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/Implementation/*.cpp)
  message(">>>>>  HEADER_FILES = ${HEADER_FILES}")
  message(">>>>>  SOURCE_FILES = ${SOURCE_FILES}")
  
# Specify resulting library.
add_library( ${PROJECT_NAME} STATIC ${SOURCE_FILES} ${HEADER_FILES})

#if(UNIT_TESTS)
  add_subdirectory(
	  ${CMAKE_CURRENT_SOURCE_DIR}/Unittest
	  ${CMAKE_CURRENT_BINARY_DIR}/Unittest
  )
#endif(UNIT_TESTS)