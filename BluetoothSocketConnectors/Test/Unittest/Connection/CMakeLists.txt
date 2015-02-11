project(BluetoothSocketConnectors_Test_Unittest_Connection)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(BluetoothSocketConnectors_Test_Unittest_Connection_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BluetoothSocketConnectors_Test_Unittest_Connection_ROOT root directory")
  #message(STATUS "*** BluetoothSocketConnectors_Test_Unittest_Connection_ROOT = ${BluetoothSocketConnectors_Test_Unittest_Connection_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify include directories.
include_directories(
    ${BluetoothSocketConnectors_Test_Unittest_API_INCLUDES}
    ${BluetoothSocketConnectors_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    ${ThirdParty_googlemock_libgtest_API_INCLUDES}
    ${ThirdParty_googlemock_libgmock_API_INCLUDES} 
    )

# Core tests
file(GLOB UNITTEST_FILES_CPP ${BluetoothSocketConnectors_Test_Unittest_Connection_ROOT}/*.cpp)
file(GLOB UNITTEST_FILES_H ${BluetoothSocketConnectors_Test_Unittest_Connection_ROOT}/*.h)
  #message(">>>>>  UNITTEST_FILES_CPP = ${UNITTEST_FILES_CPP}")
  #message(">>>>>  UNITTEST_FILES_H = ${UNITTEST_FILES_H}")
  
#append all source files
list(APPEND COMMON_UNITTEST_FILES
	${UNITTEST_FILES_CPP}
	${UNITTEST_FILES_H}
	)

# Test executable
add_executable(${PROJECT_NAME} ${COMMON_UNITTEST_FILES})

target_link_libraries(${PROJECT_NAME}
    BluetoothSocketConnectors
    gmock_main
    )
