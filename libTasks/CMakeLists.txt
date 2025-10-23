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
# Add input / output json-files
file(GLOB json_files "${CMAKE_CURRENT_SOURCE_DIR}/data/*json")

add_custom_target(JsonInput SOURCES ${json_files})

foreach(json_file ${json_files})
    get_filename_component(json_name ${json_file} NAME)
    configure_file(${json_file} "${CMAKE_BINARY_DIR}/bin/${json_name}" COPYONLY)
endforeach()
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
#-------------------------------------------------------------------------------
  target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

  add_subdirectory(tests)
#-------------------------------------------------------------------------------
add_subdirectory(dependencies/)
