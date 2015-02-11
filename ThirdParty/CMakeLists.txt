set(ThirdParty_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_ROOT directory")
  #message(STATUS "*** ThirdParty_ROOT = ${ThirdParty_ROOT}")

# googlemock & googletest
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/googlemock
	${CMAKE_CURRENT_BINARY_DIR}/googlemock)

# rapidjson 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/rapidjson 
	${CMAKE_CURRENT_BINARY_DIR}/rapidjson)


