cmake_minimum_required(VERSION 3.14)
#-------------------------------------------------------------------------------
project(SearchTask)
#-------------------------------------------------------------------------------
find_path(
  SEARCH_TASK_INCLUDE_DIR
  NAMES "SearchTask.hpp"
  PATHS ${CMAKE_CURRENT_SOURCE_DIR})
#-------------------------------------------------------------------------------
if (MSVC)
  message(STATUS "Library ${PROJECT_NAME} is supported only by Linux!")    
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(LIB_NAME libTask.so)

  file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

  file(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/${LIB_NAME}"
    DESTINATION "${CMAKE_BINARY_DIR}/bin")

  add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
  target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC "${CMAKE_BINARY_DIR}/bin/${LIB_NAME}")

  target_include_directories(${PROJECT_NAME} PUBLIC ${NLOHMANN_INCLUDE_DIR})
  
endif()
#-------------------------------------------------------------------------------
