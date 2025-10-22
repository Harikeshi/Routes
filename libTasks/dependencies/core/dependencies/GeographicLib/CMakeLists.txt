cmake_minimum_required(VERSION 2.8.12)
#-------------------------------------------------------------------------------
project(GeographicLib)
#-------------------------------------------------------------------------------
set_property(GLOBAL PROPERTY USE_FOLDERS  ON)
#-------------------------------------------------------------------------------
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeTools")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pthread")
elseif(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")
endif()
#-------------------------------------------------------------------------------
include(GroupingTools)
#-------------------------------------------------------------------------------
add_subdirectory(GeographicLib)