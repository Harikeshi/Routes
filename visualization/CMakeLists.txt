project(Visualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(${PROJECT_NAME} WIN32 main.cpp)
else ()
    add_executable(${PROJECT_NAME} main.cpp "SceneWidget.cpp" "FigureWidget.cpp" "MainWindow.cpp")
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC Routes)

find_package(Qt5 COMPONENTS Widgets
        DataVisualization
        REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Widgets
        Qt5::DataVisualization
)
