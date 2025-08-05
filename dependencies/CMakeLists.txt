if (NOT TASKS_FOUND) # Защита от повторного включения
    if (${GIT_FOUND})
        execute_process( # Подгрузка submodule из git
                COMMAND ${GIT_EXECUTABLE} submodule update --init dependencies/taskso
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        )
    endif ()

    add_subdirectory(taskso) # Добавление директории библиотеки в сборку проекта
    set(TASKS_FOUND TRUE) # Установка флага защиты от повторного включения
endif ()