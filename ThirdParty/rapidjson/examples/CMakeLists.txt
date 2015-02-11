project(rapidjson_examples)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(ThirdParty_rapidjson_examples_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "ThirdParty_rapidjson_examples root directory")
  #message(STATUS "*** ThirdParty_rapidjson_examples_ROOT = ${ThirdParty_rapidjson_examples_ROOT}")
  
# Display what's the USING_RAPIDJSON_DIR names.
  #message(STATUS "*** using variable 'USING_RAPIDJSON_DIR'=${USING_RAPIDJSON_DIR}")

set(RAPIDJSON_EX1 "condense") 
set(RAPIDJSON_EX2 "pretty")
set(RAPIDJSON_EX3 "serialize")
set(RAPIDJSON_EX4 "tutorial")
set(RAPIDJSON_EX5 "StringBufferTest")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
  
# Definition of the rapidjson_examples include files
set(ThirdParty_rapidjson_examples_API_INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/Interface
    CACHE STRING "HciSimpleScan_API_INCLUDES path")
  #message(STATUS "*** using variable 'ThirdParty_rapidjson_examples_API_INCLUDES'=${ThirdParty_rapidjson_examples_API_INCLUDES}")

# Make a list of the source files we use.
file(GLOB SOURCE_FILES_EX1 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX1}/*.cpp)
file(GLOB SOURCE_FILES_EX2 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX2}/*.cpp)
file(GLOB SOURCE_FILES_EX3 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX3}/*.cpp)
file(GLOB SOURCE_FILES_EX4 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX4}/*.cpp)
file(GLOB SOURCE_FILES_EX5 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX5}/*.cpp)

# Make a list of the header (h) files we use.
file(GLOB HEADER_FILES_EX1 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX1}/*.h)
file(GLOB HEADER_FILES_EX2 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX2}/*.h)
file(GLOB HEADER_FILES_EX3 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX3}/*.h)
file(GLOB HEADER_FILES_EX4 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX4}/*.h)
file(GLOB HEADER_FILES_EX5 ${CMAKE_CURRENT_LIST_DIR}/Implementation/${USING_RAPIDJSON_DIR}/example/${RAPIDJSON_EX5}/*.h)
  #message(">>>>>  SOURCE_FILES_EX1 = ${SOURCE_FILES_EX1}")
  #message(">>>>>  HEADER_FILES_EX1 = ${HEADER_FILES_EX1}")
  #message(">>>>>  SOURCE_FILES_EX2 = ${SOURCE_FILES_EX2}")
  #message(">>>>>  HEADER_FILES_EX2 = ${HEADER_FILES_EX2}")
  #message(">>>>>  SOURCE_FILES_EX3 = ${SOURCE_FILES_EX3}")
  #message(">>>>>  HEADER_FILES_EX3 = ${HEADER_FILES_EX3}")
  #message(">>>>>  SOURCE_FILES_EX4 = ${SOURCE_FILES_EX4}")
  #message(">>>>>  HEADER_FILES_EX4 = ${HEADER_FILES_EX4}")
  #message(">>>>>  SOURCE_FILES_EX5 = ${SOURCE_FILES_EX5}")
  #message(">>>>>  HEADER_FILES_EX5 = ${HEADER_FILES_EX5}")
  
# Specify resulting executable.
add_executable( ${PROJECT_NAME}_${RAPIDJSON_EX1} ${HEADER_FILES_EX1} ${SOURCE_FILES_EX1})
add_executable( ${PROJECT_NAME}_${RAPIDJSON_EX2} ${HEADER_FILES_EX2} ${SOURCE_FILES_EX2})
add_executable( ${PROJECT_NAME}_${RAPIDJSON_EX3} ${HEADER_FILES_EX3} ${SOURCE_FILES_EX3})
add_executable( ${PROJECT_NAME}_${RAPIDJSON_EX4} ${HEADER_FILES_EX4} ${SOURCE_FILES_EX4})
add_executable( ${PROJECT_NAME}_${RAPIDJSON_EX5} ${HEADER_FILES_EX5} ${SOURCE_FILES_EX5})

# Specify include directories.
include_directories( ${CMAKE_CURRENT_LIST_DIR}/Implementation/)
include_directories( ${CMAKE_CURRENT_LIST_DIR}/include )
include_directories(
    ${ThirdParty_rapidjson_examples_API_INCLUDES}
    ${ThirdParty_rapidjson_librapidjson_API_INCLUDES}
    )
  #message(STATUS "*** using variable 'ThirdParty_rapidjson_librapidjson_API_INCLUDES'=${ThirdParty_rapidjson_librapidjson_API_INCLUDES}")

# Specify link libs.
target_link_libraries(${PROJECT_NAME}_${RAPIDJSON_EX1} rapidjson)
target_link_libraries(${PROJECT_NAME}_${RAPIDJSON_EX2} rapidjson)
target_link_libraries(${PROJECT_NAME}_${RAPIDJSON_EX3} rapidjson)
target_link_libraries(${PROJECT_NAME}_${RAPIDJSON_EX4} rapidjson)
target_link_libraries(${PROJECT_NAME}_${RAPIDJSON_EX5} rapidjson)

#disable_warnings_as_errors_on_target(${PROJECT_NAME}_${RAPIDJSON_EX1})
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_${RAPIDJSON_EX2})
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_${RAPIDJSON_EX3})
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_${RAPIDJSON_EX4})
#disable_warnings_as_errors_on_target(${PROJECT_NAME}_${RAPIDJSON_EX5})
