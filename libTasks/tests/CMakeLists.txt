project(TaskTest)
#-------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
#-------------------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#-------------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PUBLIC ${GTEST_LIBRARIES} Task)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${GTEST_INCLUDE_DIRS}
)
#-------------------------------------------------------------------------------
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#-------------------------------------------------------------------------------
#if (MSVC)
#    group_files_by_path(ROOT_GROUP Tests)
#    group_targets_by_path(ROOT_GROUP Tests)
#endif ()
#-------------------------------------------------------------------------------
set(FROM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(TO_DIR "${CMAKE_BINARY_DIR}/bin/data/")

file(GLOB_RECURSE DATA "${FROM_DIR}/*json")
add_custom_target(data SOURCES ${DATA})

file(GLOB_RECURSE JSON_FILES
        RELATIVE "${FROM_DIR}"
        "${FROM_DIR}/*"
)

make_directory("${TO_DIR}/zigzag")
make_directory("${TO_DIR}/shift")
make_directory("${TO_DIR}/straight_tack")
make_directory("${TO_DIR}/rectangle_inners")
make_directory("${TO_DIR}/concave")
make_directory("${TO_DIR}/irregular")
make_directory("${TO_DIR}/shape_z")
make_directory("${TO_DIR}/numerous")
make_directory("${TO_DIR}/trapezoid")
make_directory("${TO_DIR}/rectangle")

foreach (file ${JSON_FILES})
    message("-- Copying json-file: ${FROM_DIR}${file} -> ${TO_DIR}${file}")
    configure_file(
            "${FROM_DIR}/${file}"
            "${TO_DIR}/${file}"
            COPYONLY
    )
endforeach ()
#-------------------------------------------------------------------------------
# target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:DEBUG>:DEBUG>)
#-------------------------------------------------------------------------------
target_compile_definitions(${PROJECT_NAME} PRIVATE BINARY_DIR="${CMAKE_BINARY_DIR}/")
#-------------------------------------------------------------------------------
#find_library(XLSXWRITER_LIB NAMES xlsxwriter)
#
#find_path(XLSXWRITER_INCLUDE_DIR NAMES xlsxwriter.h)
#
#target_include_directories(${PROJECT_NAME} PRIVATE ${XLSXWRITER_INCLUDE_DIR})
#
#target_link_libraries(${PROJECT_NAME} PRIVATE ${XLSXWRITER_LIB})


