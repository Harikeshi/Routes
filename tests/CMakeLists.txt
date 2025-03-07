project(Tests)

add_executable(${PROJECT_NAME} main.cpp
    test.h)

target_link_libraries(${PROJECT_NAME} PUBLIC Routes)
