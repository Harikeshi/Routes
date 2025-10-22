project(3rdparty)
#-------------------------------------------------------------------------------
if(${BUILD_TESTS} AND (NOT ${GTEST_FOUND}))
  # git submodules
  if(${GIT_FOUND})
    execute_process(
      COMMAND ${GIT_EXECUTABLE} submodule update --init 3rdparty/gtest
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  endif()

  add_subdirectory(gtest)
  set(GTEST_INCLUDE_DIRS "${gtest_SOURCE_DIR}/include" PARENT_SCOPE)
  set(GTEST_LIBRARY "gtest" PARENT_SCOPE)
endif()
#-------------------------------------------------------------------------------