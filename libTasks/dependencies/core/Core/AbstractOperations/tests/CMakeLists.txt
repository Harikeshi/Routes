project(AbstractOperationsTest)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCE_FILES})
#-------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTEST_LIBRARY} AbstractOperations)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PUBLIC
  ${GTEST_INCLUDE_DIRS}
)
#-------------------------------------------------------------------------------
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
#-------------------------------------------------------------------------------
if(MSVC)
  group_files_by_path(ROOT_GROUP Tests)
  group_targets_by_path(ROOT_GROUP Tests)
endif()
#-------------------------------------------------------------------------------