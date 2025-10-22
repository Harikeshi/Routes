project(ReachableAreaCalculatorTest)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#-------------------------------------------------------------------------------
find_package(Qt5 REQUIRED COMPONENTS Core)
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTEST_LIBRARY} 
	ReachableAreaCalculator Qt5::Core)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PUBLIC
  ${GTEST_INCLUDE_DIRS}
)
#-------------------------------------------------------------------------------
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
#-------------------------------------------------------------------------------
copy_resources("ReachableAreaCalculatorTestRes")
#-------------------------------------------------------------------------------
if(MSVC)
  group_files_by_path(ROOT_GROUP Tests)
  group_targets_by_path(ROOT_GROUP Tests)
endif()
#-------------------------------------------------------------------------------