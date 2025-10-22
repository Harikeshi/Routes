# Cartographic
if (NOT CARTOGRAPHIC_FOUND)
  # git submodules
  if (${GIT_FOUND})
    execute_process(
	  COMMAND ${GIT_EXECUTABLE} submodule update --init dependencies/Cartographic
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
  endif()
  add_subdirectory(Cartographic) 
  message (STATUS "include Cartographic in Core")
  set(CARTOGRAPHIC_FOUND TRUE)
endif()

# GeographicLib
if (NOT GEOGRAPHIC_LIB_FOUND)
  # git submodules
  if (${GIT_FOUND})
    execute_process(
	  COMMAND ${GIT_EXECUTABLE} submodule update --init dependencies/GeographicLib
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
  endif()
  add_subdirectory(GeographicLib) 
  message (STATUS "include GeographicLib in Core")
  set(GEOGRAPHIC_LIB_FOUND TRUE)
endif()

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
  message (STATUS "include ExceptionTools in Core")
  set(EXCEPTION_TOOLS_FOUND TRUE)
endif()

# OSTools
if (NOT OS_TOOLS_FOUND)
  # git submodules
  if (${GIT_FOUND})
    execute_process(
	  COMMAND ${GIT_EXECUTABLE} submodule update --init dependencies/OSTools
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
  endif()
  add_subdirectory(OSTools) 
  message (STATUS "include OSTools in Core")
  set(OS_TOOLS_FOUND TRUE)
endif()