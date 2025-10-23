project(Point)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
                         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${HEADER} ${SOURCE})
#-------------------------------------------------------------------------------
target_link_libraries( ${PROJECT_NAME} PUBLIC MathTools)
#target_link_libraries(${PROJECT_NAME} PRIVATE geos)
target_link_libraries(${PROJECT_NAME} PUBLIC geos)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
 #-------------------------------------------------------------------------------
if(${BUILD_TESTS})
  target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
  add_subdirectory(tests)
endif()
#-------------------------------------------------------------------------------
if(MSVC)
  group_files_by_path()
  group_targets_by_path()
endif()
#-------------------------------------------------------------------------------
