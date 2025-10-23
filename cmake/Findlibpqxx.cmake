find_path(libpqxx_INCLUDE_DIR
    NAMES pqxx/pqxx
    HINTS
        ${libs_ROOT}/pqxx/include
        #/usr/local/include
        #/usr/include
)

find_library(libpqxx_LIBRARY
    NAMES pqxx libpqxx-7.7 pqxx-7.7
    HINTS
        ${libs_ROOT}/pqxx
        #/usr/local/lib
        #/usr/lib
        #/usr/lib64
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libpqxx
    REQUIRED_VARS libpqxx_LIBRARY libpqxx_INCLUDE_DIR
    FAIL_MESSAGE "Could not find libpqxx"
)

if(libpqxx_FOUND)
    set(libpqxx_LIBRARIES ${libpqxx_LIBRARY})
    set(libpqxx_INCLUDE_DIRS ${libpqxx_INCLUDE_DIR})
endif()

message("libpqxx_INCLUDE_DIR = ${libpqxx_INCLUDE_DIR}")
message("libpqxx_LIBRARY = ${libpqxx_LIBRARY}")


mark_as_advanced(libpqxx_INCLUDE_DIR libpqxx_LIBRARY)
