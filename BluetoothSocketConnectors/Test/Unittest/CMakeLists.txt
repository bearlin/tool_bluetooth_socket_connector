project(BluetoothSocketConnectors_Test_Unittest)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(BluetoothSocketConnectors_Test_Unittest_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BluetoothSocketConnectors_Test_Unittest_ROOT root directory")
  #message(STATUS "*** BluetoothSocketConnectors_Test_Unittest_ROOT = ${BluetoothSocketConnectors_Test_Unittest_ROOT}")

set(BluetoothSocketConnectors_Test_Unittest_API_INCLUDES
  ${CMAKE_CURRENT_LIST_DIR}/MockInterface
  CACHE STRING "BluetoothSocketConnectors_Test_Unittest_API_INCLUDES path")
  #message(STATUS "*** using variable 'BluetoothSocketConnectors_Test_Unittest_API_INCLUDES'=${BluetoothSocketConnectors_Test_Unittest_API_INCLUDES}")

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Common
  ${CMAKE_CURRENT_BINARY_DIR}/Common
)

add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Connection
  ${CMAKE_CURRENT_BINARY_DIR}/Connection
)

# List of BluetoothSocketConnectors_Unittest targets
list(APPEND BluetoothSocketConnectors_Unittest_LIST
  BluetoothSocketConnectors_Test_Unittest_Common
  BluetoothSocketConnectors_Test_Unittest_Connection
)
# Add this project as a custom target, and add its dependency targets.
add_custom_target(${PROJECT_NAME} DEPENDS ${BluetoothSocketConnectors_Unittest_LIST} COMMENT "BluetoothSocketConnectors_Test_Unittest custom target")





