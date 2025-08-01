project(Tests LANGUAGES CXX)

# Для Astra изменить на 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(Qt5 REQUIRED COMPONENTS Widgets Sql Concurrent)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC Routes)

#qt5_add_resources(RESOURCES resources.qrc)
#
#target_link_libraries(${PROJECT_NAME} PUBLIC Routes Qt5::Widgets Qt5::Sql Qt5::Concurrent)