project(gmock)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(ThirdParty_googlemock_libgmock_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_googlemock_libgmock root directory")
  #message(STATUS "*** ThirdParty_googlemock_libgmock_ROOT = ${ThirdParty_googlemock_libgmock_ROOT}")

# Definition of the googlemock/googletest include files
set(GOOGLETEST_INCLUDES
	${CMAKE_CURRENT_LIST_DIR}/Interface
	CACHE STRING "googletest API INCLUDES path")
  #message(STATUS "*** using variable 'GOOGLETEST_INCLUDES'=${GOOGLETEST_INCLUDES}")
  
set(GOOGLEMOCK_INCLUDES
	${CMAKE_CURRENT_LIST_DIR}/Interface
	CACHE STRING "googlemock API INCLUDES path")
  #message(STATUS "*** using variable 'GOOGLEMOCK_INCLUDES'=${GOOGLEMOCK_INCLUDES}")

set(GOOGLETEST_INCLUDES_SRC
  ${CMAKE_CURRENT_LIST_DIR}/Implementation/gtest
	CACHE STRING "googletest API INCLUDES path")
  #message(STATUS "*** using variable 'GOOGLETEST_INCLUDES_SRC'=${GOOGLETEST_INCLUDES_SRC}")
  
set(GOOGLEMOCK_INCLUDES_SRC
	${CMAKE_CURRENT_LIST_DIR}/Implementation/gmock
	CACHE STRING "googlemock API INCLUDES path")
  #message(STATUS "*** using variable 'GOOGLEMOCK_INCLUDES_SRC'=${GOOGLEMOCK_INCLUDES_SRC}")

set(ThirdParty_googlemock_libgtest_API_INCLUDES
    ${GOOGLETEST_INCLUDES}
    ${GOOGLETEST_INCLUDES_SRC}
    CACHE STRING "ThirdParty_googlemock_libgtest_API_INCLUDES path")
  #message(STATUS "*** using variable 'ThirdParty_googlemock_libgtest_API_INCLUDES'=${ThirdParty_googlemock_libgtest_API_INCLUDES}")

set(ThirdParty_googlemock_libgmock_API_INCLUDES
    ${GOOGLEMOCK_INCLUDES}
    ${GOOGLEMOCK_INCLUDES_SRC}
    CACHE STRING "ThirdParty_googlemock_libgmock_API_INCLUDES path")
  #message(STATUS "*** using variable 'ThirdParty_googlemock_libgmock_API_INCLUDES'=${ThirdParty_googlemock_libgmock_API_INCLUDES}")

#Include path
include_directories( 
  ${ThirdParty_googlemock_libgtest_API_INCLUDES} 
  ${ThirdParty_googlemock_libgmock_API_INCLUDES} 
  )

# gtest Source files
file_glob( GOOGLEMOCK_GTEST_ALL_CPP_FILE ${CMAKE_CURRENT_LIST_DIR}/Implementation/gtest/src/gtest-all.cc)
  #message(STATUS "*** using variable 'GOOGLEMOCK_GTEST_ALL_CPP_FILE'=${GOOGLEMOCK_GTEST_ALL_CPP_FILE}")
file_glob( GOOGLEMOCK_GTEST_MAIN_CPP_FILE ${CMAKE_CURRENT_LIST_DIR}/Implementation/gtest/src/gtest_main.cc)
  #message(STATUS "*** using variable 'GOOGLEMOCK_GTEST_MAIN_CPP_FILE'=${GOOGLEMOCK_GTEST_MAIN_CPP_FILE}")

# gmock Source files
file_glob( GOOGLEMOCK_GMOCK_ALL_CPP_FILE ${CMAKE_CURRENT_LIST_DIR}/Implementation/gmock/src/gmock-all.cc)
  #message(STATUS "*** using variable 'GOOGLEMOCK_GMOCK_ALL_CPP_FILE'=${GOOGLEMOCK_GMOCK_ALL_CPP_FILE}")
file_glob( GOOGLEMOCK_GMOCK_MAIN_CPP_FILE ${CMAKE_CURRENT_LIST_DIR}/Implementation/gmock/src/gmock_main.cc)
  #message(STATUS "*** using variable 'GOOGLEMOCK_GMOCK_MAIN_CPP_FILE'=${GOOGLEMOCK_GMOCK_MAIN_CPP_FILE}")

# On platforms where the pthread library is available, Google Test and Google Mock uses it in order to be thread-safe, more details see gtest-1.7.0/README and gmock-1.7.0/README.
find_library(PTHREAD_LIB pthread)
if (${PTHREAD_LIB} STREQUAL "PTHREAD_LIB-NOTFOUND")
  message(STATUS "*** Not found variable 'PTHREAD_LIB', libgtest and libgmock won't be thread-safe.")
else()
  message(STATUS "*** using variable 'PTHREAD_LIB'=${PTHREAD_LIB}")
endif()

#libgtest.a and libgtest_main.a
add_library(gtest STATIC ${GOOGLEMOCK_GTEST_ALL_CPP_FILE})
target_link_libraries(gtest ${PTHREAD_LIB})
add_library(gtest_main STATIC ${GOOGLEMOCK_GTEST_MAIN_CPP_FILE})
target_link_libraries(gtest_main gtest)

#libgmock.a and libgmock_main.a
add_library(${PROJECT_NAME} STATIC ${GOOGLEMOCK_GMOCK_ALL_CPP_FILE})
target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIB} gtest)
add_library(${PROJECT_NAME}_main STATIC ${GOOGLEMOCK_GMOCK_MAIN_CPP_FILE})
target_link_libraries(${PROJECT_NAME}_main ${PROJECT_NAME})

#disable_warnings_as_errors_on_target(gtest)
#disable_warnings_as_errors_on_target(gtest_main)
#disable_warnings_as_errors_on_target(${PROJECT_NAME})
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_main)
