#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.16)
#-------------------------------------------------------------------------------
project(Task VERSION 1.0.0 LANGUAGES C CXX)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE HEADER "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
                         "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
#-------------------------------------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${HEADER} ${SOURCE})
#-------------------------------------------------------------------------------
if(MSVC)
    find_package(nlohmann_json REQUIRED)
    if(nlohmann_json_DIR)
        target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
    endif()
endif()
#-------------------------------------------------------------------------------
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
#-------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} ExceptionTools MathTools Point PrimaryEntities Geometry TriangularGeometry)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
#-------------------------------------------------------------------------------
add_subdirectory(dependencies/)
