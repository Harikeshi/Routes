# ExceptionTools
if (NOT EXCEPTION_TOOLS_FOUND)
  # git submodules
  if (${GIT_FOUND})
    execute_process(
	  COMMAND ${GIT_EXECUTABLE} submodule update --init dependencies/ExceptionTools
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
  endif()
  add_subdirectory(ExceptionTools) 
  message (STATUS "include ExceptionTools in OSTools")
  set(EXCEPTION_TOOLS_FOUND TRUE)
endif()


