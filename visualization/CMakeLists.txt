project(Visualization LANGUAGES CXX)

# Для Astra изменить на 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Concurrent)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Routes)

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