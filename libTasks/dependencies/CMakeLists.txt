if (NOT CORE_FOUND)
    # git submodules
    if (${GIT_FOUND})
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init dependencies/core
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
          )
    endif()
   add_subdirectory(core)
    message (STATUS "include Core in libTask")
    set(CORE_FOUND TRUE)
endif()
