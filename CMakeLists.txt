cmake_minimum_required(VERSION 2.8)

project(tool_bluetooth_socket_connector)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

## -fPIC: http://stackoverflow.com/questions/5311515/gcc-fpic-option
## -m32: http://stackoverflow.com/questions/2426478/when-should-m32-option-of-gcc-be-used

# Build x86 binary 
# Use "$ sudo apt-get install libbluetooth-dev:i386" to install m32 BlueZ library.
set(CMAKE_CXX_FLAGS "-Wall -Werror -m32 -fPIC") 
set(CMAKE_C_FLAGS "-Wall -Werror -m32 -fPIC") 
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/lib/i386-linux-gnu) # This is important to tell CMAKE to find those 64-bits libraries.

# Build x64 binary 
# Use "$ sudo apt-get install libbluetooth-dev" to install m64 BlueZ library.
#set(CMAKE_CXX_FLAGS "-Wall -Werror -m64 -fPIC") 
#set(CMAKE_C_FLAGS "-Wall -Werror -m64 -fPIC") 
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/lib/x86_64-linux-gnu) # This is important to tell CMAKE to find those 64-bits libraries.

set(tool_bluetooth_socket_connector_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "tool_bluetooth_socket_connector root directory")
  #message(STATUS "*** tool_bluetooth_socket_connector_ROOT = ${tool_bluetooth_socket_connector_ROOT}")

include(${CMAKE_CURRENT_LIST_DIR}/CMake-Utils.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ThirdParty 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty 
	${CMAKE_CURRENT_BINARY_DIR}/ThirdParty)

# Imports
add_subdirectory(
        ${PROJECT_SOURCE_DIR}/Imports
        ${PROJECT_BINARY_DIR}/Imports)

# BluetoothSocketConnectors 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/BluetoothSocketConnectors 
	${CMAKE_CURRENT_BINARY_DIR}/BluetoothSocketConnectors)
