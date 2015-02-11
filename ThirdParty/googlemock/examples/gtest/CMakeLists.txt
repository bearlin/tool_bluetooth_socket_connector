project(gtest_examples)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(ThirdParty_googlemock_examples_gtest_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_googlemock_examples_gtest_ROOT root directory")
  #message(STATUS "*** ThirdParty_googlemock_examples_gtest_ROOT = ${ThirdParty_googlemock_examples_gtest_ROOT}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
# Definition of the googlemock_examples include files
set(ThirdParty_googlemock_examples_gtest_API_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/Interface
    ${CMAKE_CURRENT_LIST_DIR}/Implementation
    ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples
    CACHE STRING "ThirdParty_googlemock_examples_gtest_API_INCLUDES path")
  #message(STATUS "*** using variable 'ThirdParty_googlemock_examples_gtest_API_INCLUDES'=${ThirdParty_googlemock_examples_gtest_API_INCLUDES}")
  
# Make a list of the source files we use.
file(GLOB SOURCE_FILES_EX1 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample1.cc ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample1_*.cc)
file(GLOB SOURCE_FILES_EX2 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample2*.cc)
file(GLOB SOURCE_FILES_EX3 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample3*.cc)
file(GLOB SOURCE_FILES_EX4 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample4*.cc)
file(GLOB SOURCE_FILES_EX5 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample5*.cc ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample1.cc)
file(GLOB SOURCE_FILES_EX6 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample6*.cc)
file(GLOB SOURCE_FILES_EX7 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample7*.cc)
file(GLOB SOURCE_FILES_EX8 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample8*.cc)
file(GLOB SOURCE_FILES_EX9 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample9*.cc)
file(GLOB SOURCE_FILES_EX10 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample10*.cc)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES_EX1 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample1*.h)
file(GLOB HEADER_FILES_EX2 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample2*.h)
file(GLOB HEADER_FILES_EX3 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample3*.h)
file(GLOB HEADER_FILES_EX4 ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/sample4*.h)
file(GLOB HEADER_FILES_TAB ${CMAKE_CURRENT_LIST_DIR}/Implementation/samples/prime*.h)
  #message(">>>>>  SOURCE_FILES_EX1 = ${SOURCE_FILES_EX1}")
  #message(">>>>>  SOURCE_FILES_EX2 = ${SOURCE_FILES_EX2}")
  #message(">>>>>  SOURCE_FILES_EX3 = ${SOURCE_FILES_EX3}")
  #message(">>>>>  SOURCE_FILES_EX4 = ${SOURCE_FILES_EX4}")
  #message(">>>>>  SOURCE_FILES_EX5 = ${SOURCE_FILES_EX5}")
  #message(">>>>>  SOURCE_FILES_EX6 = ${SOURCE_FILES_EX6}")
  #message(">>>>>  SOURCE_FILES_EX7 = ${SOURCE_FILES_EX7}")
  #message(">>>>>  SOURCE_FILES_EX8 = ${SOURCE_FILES_EX8}")
  #message(">>>>>  SOURCE_FILES_EX9 = ${SOURCE_FILES_EX9}")
  #message(">>>>>  SOURCE_FILES_EX10 = ${SOURCE_FILES_EX10}")
  #message(">>>>>  HEADER_FILES_EX1 = ${HEADER_FILES_EX1}")
  #message(">>>>>  HEADER_FILES_EX2 = ${HEADER_FILES_EX2}")
  #message(">>>>>  HEADER_FILES_EX3 = ${HEADER_FILES_EX3}")
  #message(">>>>>  HEADER_FILES_EX4 = ${HEADER_FILES_EX4}")
  #message(">>>>>  HEADER_FILES_TAB = ${HEADER_FILES_TAB}")

# Specify resulting executable.
add_executable( ${PROJECT_NAME}_sample1_unittest ${HEADER_FILES_EX1} ${SOURCE_FILES_EX1})
add_executable( ${PROJECT_NAME}_sample2_unittest ${HEADER_FILES_EX2} ${SOURCE_FILES_EX2})
add_executable( ${PROJECT_NAME}_sample3_unittest ${HEADER_FILES_EX3} ${SOURCE_FILES_EX3})
add_executable( ${PROJECT_NAME}_sample4_unittest ${HEADER_FILES_EX4} ${SOURCE_FILES_EX4})
add_executable( ${PROJECT_NAME}_sample5_unittest ${SOURCE_FILES_EX5})
add_executable( ${PROJECT_NAME}_sample6_unittest ${SOURCE_FILES_EX6})
add_executable( ${PROJECT_NAME}_sample7_unittest ${SOURCE_FILES_EX7})
add_executable( ${PROJECT_NAME}_sample8_unittest ${SOURCE_FILES_EX8})
add_executable( ${PROJECT_NAME}_sample9_unittest ${SOURCE_FILES_EX9})
add_executable( ${PROJECT_NAME}_sample10_unittest ${SOURCE_FILES_EX10})

# Specify include directories.
include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )
include_directories(
    ${ThirdParty_googlemock_examples_gtest_API_INCLUDES}
    ${ThirdParty_googlemock_libgtest_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'ThirdParty_googlemock_libgmock_API_INCLUDES'=${ThirdParty_googlemock_libgmock_API_INCLUDES}")

# Specify link libs.
target_link_libraries(${PROJECT_NAME}_sample1_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample2_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample3_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample4_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample5_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample6_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample7_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample8_unittest gtest_main)
target_link_libraries(${PROJECT_NAME}_sample9_unittest gtest)
target_link_libraries(${PROJECT_NAME}_sample10_unittest gtest)

#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample1_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample2_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample3_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample4_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample5_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample6_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample7_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample8_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample9_unittest)
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_sample10_unittest)

