project(Imports_Logging_Unittest_LoggingTest)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(Imports_Logging_Unittest_LoggingTest_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "Imports_Logging_Unittest_LoggingTest_ROOT root directory")
  message(STATUS "*** Imports_Logging_Unittest_LoggingTest_ROOT = ${Imports_Logging_Unittest_LoggingTest_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify include directories.
include_directories(
    ${Imports_Logging_API_INCLUDES}
    ${ThirdParty_googlemock_libgtest_API_INCLUDES}
    ${ThirdParty_googlemock_libgmock_API_INCLUDES} 
    )

# Core tests
file(GLOB UNITTEST_FILES_CPP ${Imports_Logging_Unittest_LoggingTest_ROOT}/*.cpp)
file(GLOB UNITTEST_FILES_H ${Imports_Logging_Unittest_LoggingTest_ROOT}/*.h)
  message(">>>>>  UNITTEST_FILES_CPP = ${UNITTEST_FILES_CPP}")
  message(">>>>>  UNITTEST_FILES_H = ${UNITTEST_FILES_H}")
  
#append all source files
list(APPEND COMMON_UNITTEST_FILES
	${UNITTEST_FILES_CPP}
	${UNITTEST_FILES_H}
	)

# Test executable
add_executable(${PROJECT_NAME} ${COMMON_UNITTEST_FILES})

target_link_libraries(${PROJECT_NAME}
    Imports_Logging
    gmock_main
    )
