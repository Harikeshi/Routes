cmake_minimum_required(VERSION 3.16)
#-------------------------------------------------------------------------------
project(Routes VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

if (NOT MSVC)
    set(libs_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/libs")
    set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
endif ()

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

#-------------------------------------------------------------------------------
if (MSVC)
    set(Qt5_DIR "C:/Qt/5.15.17-msvc2019_64/lib/cmake/Qt5")
endif ()
find_package(Qt5 COMPONENTS Core Gui Widgets DataVisualization REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::DataVisualization
)
#-------------------------------------------------------------------------------
add_subdirectory(libTasks/)
target_link_libraries(${PROJECT_NAME} PUBLIC Task)
#-------------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
#-------------------------------------------------------------------------------
add_subdirectory(visualization/)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/visualization")
#-------------------------------------------------------------------------------
find_package(libpqxx REQUIRED)

if (MSVC)
    target_link_libraries(${PROJECT_NAME} PUBLIC libpqxx::pqxx)
else ()
    target_include_directories(${PROJECT_NAME} PUBLIC ${libpqxx_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${libpqxx_LIBRARIES})
endif ()
#-------------------------------------------------------------------------------
