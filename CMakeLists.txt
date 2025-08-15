cmake_minimum_required(VERSION 3.16)
#-------------------------------------------------------------------------------
project(Routes VERSION 1.0.0 LANGUAGES CXX)

file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Для Astra C++17
set(CMAKE_CXX_STANDARD 20)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES}
        include/Project/MainWindow.hpp)
find_package(Git)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_subdirectory(visualization/)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/visualization")

target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Widgets Qt5::Core Qt5::Gui)

# Функция для определения, является ли система Arch Linux
function(check_arch_linux)
    if (EXISTS "/etc/os-release")
        file(READ "/etc/os-release" os_release)
        if (os_release MATCHES "ID=arch")
            set(IS_ARCH_LINUX TRUE PARENT_SCOPE)
        endif ()
    endif ()
endfunction()

# Проверяем
check_arch_linux()

if (IS_ARCH_LINUX)
    # Если CMake не нашел libpqxx, пробуем вручную
    find_path(LIBPQXX_INCLUDE_DIR pqxx/pqxx)
    find_library(LIBPQXX_LIBRARY pqxx)
    find_library(LIBPQ_LIBRARY pq)

    if (LIBPQXX_INCLUDE_DIR AND LIBPQXX_LIBRARY AND LIBPQ_LIBRARY)
        add_library(libpqxx INTERFACE IMPORTED)
        target_include_directories(libpqxx INTERFACE ${LIBPQXX_INCLUDE_DIR})
        target_link_libraries(libpqxx INTERFACE ${LIBPQXX_LIBRARY} ${LIBPQ_LIBRARY})
        target_link_libraries(${PROJECT_NAME} PUBLIC libpqxx)
    else ()
        message(FATAL_ERROR "libpqxx not found!")
    endif ()
else ()
    find_package(libpqxx REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC libpqxx::pqxx)
endif ()

find_package(PostgreSQL REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC PostgreSQL::PostgreSQL)

target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs)

if (IS_ARCH_LINUX)
    target_link_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/arch)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/arch)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/arch/libTask.so)
elseif (UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/libTask.so)
else ()
    #    target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/Task.dll)
    if (DEBUG)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/Task_d.lib)
    else ()
        target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/libs/Task.lib)
    endif ()
endif ()