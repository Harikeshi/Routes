project(ReachableAreaCalculator)
#-------------------------------------------------------------------------------
find_package(Qt5 REQUIRED COMPONENTS Core)
#-------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fext-numeric-literals")
endif()
#-------------------------------------------------------------------------------
file(GLOB_RECURSE HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
						 "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${HEADER} ${SOURCE})
#-------------------------------------------------------------------------------
target_link_libraries(
  ${PROJECT_NAME}
  Point Geometry Geodesic Qt5::Core)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PRIVATE 
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
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