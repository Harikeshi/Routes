project(Visualization LANGUAGES CXX)

# Для Astra изменить на 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(${PROJECT_NAME} WIN32 main.cpp)
else ()
    add_executable(${PROJECT_NAME} main.cpp)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC Routes)

if (MSVC)
    set(Qt5_DIR "C:/Qt/5.15.17-msvc2019_64/lib/cmake/Qt5")
endif ()

find_package(Qt5 COMPONENTS Widgets
        DataVisualization
        REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Widgets
        Qt5::DataVisualization
)
#
#if (MSVC)
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${CMAKE_SOURCE_DIR}/libs/Task.dll"
#            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#    )
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${CMAKE_SOURCE_DIR}/libs/Task.lib"
#            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#    )
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${CMAKE_SOURCE_DIR}/libs/Task_d.dll"
#            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#    )
#    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${CMAKE_SOURCE_DIR}/libs/Task_d.lib"
#            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#    )
#endif ()


