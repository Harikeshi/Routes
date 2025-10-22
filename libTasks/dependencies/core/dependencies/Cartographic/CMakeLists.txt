cmake_minimum_required(VERSION 2.8.12)
#-------------------------------------------------------------------------------
project(Cartographic)
#-------------------------------------------------------------------------------
set(CARTOGRAPHIC_MAJOR_VERSION 0)
set(CARTOGRAPHIC_MINOR_VERSION 0)
set(CARTOGRAPHIC_PATCH_VERSION 5)
#-------------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS  ON)
#-------------------------------------------------------------------------------
option(BUILD_TESTS "Build test with gtest" OFF)
#-------------------------------------------------------------------------------
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeTools")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")
  set(CMAKE_CXX_STANDARD 11) 
endif()
#-------------------------------------------------------------------------------
include(GroupingTools)
#-------------------------------------------------------------------------------
# gtest
find_package(GTest)

# git
find_package(Git)

if(NOT ${GTEST_FOUND})
  mark_as_advanced(
    gmock_build_tests
    gtest_build_samples
    gtest_build_tests
    gtest_disable_pthreads
    gtest_force_shared_crt
  )
endif()

if(${BUILD_TESTS})
  enable_testing()
  message(STATUS "[${PROJECT_NAME}] Build tests: ON")
else()
  message(STATUS "[${PROJECT_NAME}] Build tests: OFF")
endif()
#-------------------------------------------------------------------------------
add_subdirectory(3rdparty)
add_subdirectory(Cartographic)