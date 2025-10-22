project(GeographicLib)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${HEADER} ${SOURCE})
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
#-------------------------------------------------------------------------------
if(MSVC)
  group_files_by_path()
  group_targets_by_path()
endif()
#-------------------------------------------------------------------------------