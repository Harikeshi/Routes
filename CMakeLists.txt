cmake_minimum_required(VERSION 3.16)
#-------------------------------------------------------------------------------
project(Routes VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Для Astra C++17
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
find_package(Git)
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(tests/)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/tests")

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui Task)

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC libpqxx::pqxx)

find_package(PostgreSQL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC PostgreSQL::PostgreSQL)

add_subdirectory(dependencies)
# MINGW ---------------------------------------------------------------------------------
#find_package(nlohmann_json REQUIRED)
#target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json)
#find_package(libpqxx CONFIG REQUIRED)
#set(MINGW_ROOT "C:/msys64/mingw64")
#set(PostgreSQL_INCLUDE_DIR "${MINGW_ROOT}/include")
#set(PostgreSQL_LIBRARY "${MINGW_ROOT}/lib/libpq.dll.a")
#set(PQXX_INCLUDE_DIR "${MINGW_ROOT}/include")
#set(PQXX_LIBRARY "${MINGW_ROOT}/lib/libpqxx.dll.a")
# ---------------------------------------------------------------------------------------
# MSVC ----------------------------------------------------------------------------------
#    find_package(libpqxx CONFIG REQUIRED)
#    set(PQXX_INCLUDE_DIR "C:/vcpkg/installed/x64-windows/include")
#    set(PQXX_LIBRARY "C:/vcpkg/installed/x64-windows/lib/pqxx.lib")
#
#    set(PostgreSQL_INCLUDE_DIR "C:/vcpkg/installed/x64-windows/include")
#    set(PostgreSQL_LIBRARY "C:/vcpkg/installed/x64-windows/lib/libpq.lib")
#
# ---------------------------------------------------------------------------------------
# Astra ---------------------------------------------------------------------------------
#    find_package(PostgreSQL REQUIRED)
#    find_package(libpqxx REQUIRED)
#
#    target_link_libraries(${PROJECT_NAME} PUBLIC pqxx)
#
# ---------------------------------------------------------------------------------------
#if (WIN32)
#    # Для username
#    target_link_libraries(${PROJECT_NAME} PUBLIC DbgHelp)
#endif ()

#target_include_directories(${PROJECT_NAME} PUBLIC ${PQXX_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} PUBLIC ${PQXX_LIBRARY})
#target_include_directories(${PROJECT_NAME} PUBLIC ${PostgreSQL_INCLUDE_DIR})
#target_link_libraries(${PROJECT_NAME} PUBLIC ${PostgreSQL_LIBRARY})