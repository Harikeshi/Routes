project(Visualization LANGUAGES CXX)

# Для Astra изменить на 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Concurrent)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Routes)

# Найти Qt6 сначала, иначе Qt5
#find_package(Qt6 COMPONENTS Widgets DataVisualization REQUIRED QUIET)
#if (Qt6_FOUND)
#    message(STATUS "Using Qt6")
#    qt_add_executable(QtSurfaceDemo
#            main.cpp
#    )
#    target_link_libraries(QtSurfaceDemo PRIVATE
#            Qt6::Widgets
#            Qt6::DataVisualization
#    )
#else ()
#    message(STATUS "Falling back to Qt5")
    find_package(Qt5 COMPONENTS Widgets DataVisualization REQUIRED)
#    add_executable(QtSurfaceDemo
#            main.cpp
#    )
    target_link_libraries(Visualization PUBLIC
            Qt5::Widgets
            Qt5::DataVisualization
    )
#endif ()

if (MSVC)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/Task.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/Task.lib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/Task_d.dll"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/libs/Task_d.lib"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )

endif ()


#qt5_add_resources(RESOURCES resources.qrc)
#
#target_link_libraries(${PROJECT_NAME} PUBLIC Routes Qt5::Widgets Qt5::Sql Qt5::Concurrent)
