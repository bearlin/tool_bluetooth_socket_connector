# ====================================================================================================================
# Two simple macros that when called with an unset or empty (environment) variable will halt the build.
# Call examples:
# CmakeCheckVar( NameOfSomeVariable)
# CmakeCheckEnvVar( NameOfEnvironmentVariable)
# ====================================================================================================================
macro(CmakeCheckVar var)
  if( NOT DEFINED ${var})
    message( FATAL_ERROR "variable ${var} is not set.")
  endif()
  if (${var} STREQUAL "")
    message( FATAL_ERROR "variable ${var} is empty.")
  endif()
  # message( "Variable ${var} has value ${${var}}")
endmacro()

macro(CmakeCheckEnvVar var)
  #message( "env name ${var} value $ENV{${var}}")
  set( what $ENV{${var}})
  #message( "what = ${what} ${${what}}")
  if( "${what}" STREQUAL "")
    message( FATAL_ERROR "Environment variable ${var} is empty or not set.")
  endif()
endmacro()

macro(Halt)
  message( "")
  message( "++++++++++++++++++++++++++++++++++++++++++++")
  message( "STOP STOP STOP STOP STOP STOP STOP STOP STOP")
  message( "++++++++++++++++++++++++++++++++++++++++++++")
  message( "")
  message( FATAL_ERROR "Halt called.")
endmacro()

############################################################################################################
# Function for a better printfing system per component
############################################################################################################
function(print QUALCOSA)
	if(GENERATION_MESSAGES)
		message(STATUS "[${PROJECT_NAME}] ${QUALCOSA}")
	endif(GENERATION_MESSAGES)
endfunction(print QUALCOSA)

############################################################################################################
# This function strips out the '-Werror" flag from the current C and C++ compiler flags.
# This should only be used for external projects which contain warnings we are not in control of.
# Remember to re-enable them after your particular module. Note this relies on specific ordering of flags
# in the default setup, so be careful when changing them.
############################################################################################################
macro(disable_warnings_as_errors_on_target PROJECT_NAME)
  if(UNIX)
    print("Disabling warning as error in ${PROJECT_NAME}")
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-Wno-error")
  endif(UNIX)
endmacro(disable_warnings_as_errors_on_target PROJECT_NAME)

###############################################################################
# Name: Wrapping function that has same functionalities of file(GLOB ...
#       but ensure that the list is returned in alphabetical order
#       Note that passed arguments are not explicitly listed; this function
#       uses ARGN to pass variable number of arguments
# Parameter:
#  VAR: variable of the resultant list
#  ARGN: variadic number of patterns
###############################################################################
function(file_glob VAR)
  file(GLOB LOCAL_VAR ${ARGN})
  list(SORT LOCAL_VAR)
  set(${VAR} ${LOCAL_VAR} PARENT_SCOPE)
endfunction()

###############################################################################
# Name: Wrapping function that has same functionalities of file(GLOB_RECURSE
#       but ensure that the list is returned in alphabetical order
#       Note that passed arguments are not explicitly listed; this function
#       uses ARGN to pass variable number of arguments
# Parameter:
#  VAR: variable of the resultant list
#  ARGN: variadic number of patterns
###############################################################################
function(file_glob_recurse VAR)
  file(GLOB_RECURSE LOCAL_VAR ${ARGN})
  list(SORT LOCAL_VAR)
  set(${VAR} ${LOCAL_VAR} PARENT_SCOPE)
endfunction()
