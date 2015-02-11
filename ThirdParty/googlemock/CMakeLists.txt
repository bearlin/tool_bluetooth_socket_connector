set(ThirdParty_googlemock_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_googlemock_ROOT directory")
  #message(STATUS "*** ThirdParty_googlemock_ROOT = ${ThirdParty_googlemock_ROOT}")
  
# libgmock 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/libgmock 
	${CMAKE_CURRENT_BINARY_DIR}/libgmock)

# examples 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/examples 
	${CMAKE_CURRENT_BINARY_DIR}/examples)
