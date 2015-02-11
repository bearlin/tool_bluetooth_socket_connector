set(ThirdParty_rapidjson_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_rapidjson_ROOT directory")
  #message(STATUS "*** ThirdParty_rapidjson_ROOT = ${ThirdParty_rapidjson_ROOT}")
  
set(USING_RAPIDJSON_DIR "rapidjson")
#set(USING_RAPIDJSON_DIR "rapidjson-0.11")
  #message(STATUS "*** using variable 'USING_RAPIDJSON_DIR '=${USING_RAPIDJSON_DIR}")
  
# librapidjson 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/librapidjson 
	${CMAKE_CURRENT_BINARY_DIR}/librapidjson)

# examples 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/examples 
	${CMAKE_CURRENT_BINARY_DIR}/examples)
