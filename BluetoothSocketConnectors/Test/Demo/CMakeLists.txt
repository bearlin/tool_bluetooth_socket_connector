project(BluetoothSocketConnectors_Test_Demo)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(BluetoothSocketConnectors_Test_Demo_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BluetoothSocketConnectors_Test_Demo_ROOT root directory")
  #message(STATUS "*** BluetoothSocketConnectors_Test_Demo_ROOT = ${BluetoothSocketConnectors_Test_Demo_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Specify include directories.
include_directories(
    ${BluetoothSocketConnectors_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'BluetoothSocketConnectors_API_INCLUDES'=${BluetoothSocketConnectors_API_INCLUDES}")

# Core tests
file(GLOB DEMO_FILES_CPP ${BluetoothSocketConnectors_Test_Demo_ROOT}/*.cpp)
file(GLOB DEMO_FILES_H ${BluetoothSocketConnectors_Test_Demo_ROOT}/*.h)
  #message(">>>>>  DEMO_FILES_CPP = ${DEMO_FILES_CPP}")
  #message(">>>>>  DEMO_FILES_H = ${DEMO_FILES_H}")
  
#append all source files
list(APPEND COMMON_DEMO_FILES
	${DEMO_FILES_CPP}
	${DEMO_FILES_H}
	)

# Test executable
add_executable(${PROJECT_NAME} ${COMMON_DEMO_FILES})

target_link_libraries(${PROJECT_NAME}
    BluetoothSocketConnectors
    )
