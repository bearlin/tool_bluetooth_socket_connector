set(ThirdParty_googlemock_examples_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "BTServices Tools ThirdParty googlemock examples root directory")
  #message(STATUS "*** ThirdParty_googlemock_examples_ROOT = ${ThirdParty_googlemock_examples_ROOT}")
  
# gtest 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/gtest 
	${CMAKE_CURRENT_BINARY_DIR}/gtest)

# gmock 
add_subdirectory(
	${CMAKE_CURRENT_SOURCE_DIR}/gmock 
	${CMAKE_CURRENT_BINARY_DIR}/gmock)
