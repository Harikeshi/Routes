find_path(LIBPQXX_INCLUDE_DIR
        NAMES pqxx/pqxx
        PATHS /usr/include
)

find_library(LIBPQXX_LIBRARY
        NAMES pqxx
        PATHS /usr/lib/x86_64-linux-gnu
)
message("-- LIB_PQXX_DIR: ${LIBPQXX_INCLUDE_DIR}")
message("-- LIB_PQXX: ${LIBPQXX_LIBRARY}")


include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(pqxx
        REQUIRED_VARS
        LIBPQXX_INCLUDE_DIR LIBPQXX_LIBRARY)

if (LIBQXX_FOUND)
    set(LIBPQXX_INCLUDE_DIRS ${LIBPQXX_INCLUDE_DIR})
    set(LIBPQXX_LIBRARIES ${LIBPQXX_LIBRARY} pq)

    mark_as_advanced(LIBPQXX_INCLUDE_DIR LIBPQXX_LIBRARY)
endif ()
