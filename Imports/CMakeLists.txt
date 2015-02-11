project(Imports)
  message(STATUS "*** PROJECT_NAME = ${PROJECT_NAME}")

set(Imports_ROOT ${CMAKE_CURRENT_LIST_DIR} CACHE STRING "Imports_ROOT root directory")
  message(STATUS "*** Imports_ROOT = ${Imports_ROOT}")

# Logging
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/Logging
  ${CMAKE_CURRENT_BINARY_DIR}/Logging
)

# Add this project as a custom target, and add its dependency targets.
add_custom_target(${PROJECT_NAME} COMMENT "Imports custom target")
add_dependencies(${PROJECT_NAME} Imports_Logging)
