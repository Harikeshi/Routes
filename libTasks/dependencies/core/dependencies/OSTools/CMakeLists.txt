cmake_minimum_required(VERSION 2.8.12)
#-------------------------------------------------------------------------------
project(OSTools)
#-------------------------------------------------------------------------------
set(OS_TOOLS_MAJOR_VERSION 1)
set(OS_TOOLS_MINOR_VERSION 0)
set(OS_TOOLS_PATCH_VERSION 0)
#-------------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS  ON)
#-------------------------------------------------------------------------------
option(BUILD_TESTS "Build test with gtest" OFF)
#-------------------------------------------------------------------------------
set(CMAKE_DEBUG_POSTFIX "_d")
#-------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeTools")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread -fext-numeric-literals")
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4 /bigobj")
  set(CMAKE_CXX_STANDARD 11) 
endif()
#-------------------------------------------------------------------------------
include(GroupingTools)
include(CopyResources)
#-------------------------------------------------------------------------------
# git
find_package(Git)
#-------------------------------------------------------------------------------
# gtest
find_package(GTest)
#-------------------------------------------------------------------------------
if(NOT ${GTEST_FOUND})
  mark_as_advanced(
    gmock_build_tests
    gtest_build_samples
    gtest_build_tests
    gtest_disable_pthreads
    gtest_force_shared_crt
  )
endif()
#-------------------------------------------------------------------------------
if(${BUILD_TESTS})
  enable_testing()
  message(STATUS "[${PROJECT_NAME}] Build tests: ON")
else()
  message(STATUS "[${PROJECT_NAME}] Build tests: OFF")
endif()
#-------------------------------------------------------------------------------
add_subdirectory(3rdparty)
add_subdirectory(dependencies)
add_subdirectory(OSTools)
