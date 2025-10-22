#-------------------------------------------------------------------------------
if(${BUILD_TESTS} AND (NOT ${GTEST_FOUND}))
  # git submodules
  if(${GIT_FOUND})
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init 3rdparty/gtest
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )
  endif()

  add_subdirectory(gtest)
  set(GTEST_INCLUDE_DIRS "${gtest_SOURCE_DIR}/include" PARENT_SCOPE)
  set(GTEST_LIBRARY "gtest" PARENT_SCOPE)
endif()
#-------------------------------------------------------------------------------
# GEOS
if (NOT GEOS_FOUND)
  # git submodules
  if (${GIT_FOUND})
    execute_process(
	  COMMAND ${GIT_EXECUTABLE} submodule update --init 3rdparty/GEOS
	  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	)
  endif()
  add_subdirectory(GEOS) 
  message (STATUS "include GEOS in Core")
  set(GEOS_FOUND TRUE)
endif()
#-------------------------------------------------------------------------------