# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Routes_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Routes_autogen.dir/ParseCache.txt"
  "Routes_autogen"
  "tests/CMakeFiles/Tests_autogen.dir/AutogenUsed.txt"
  "tests/CMakeFiles/Tests_autogen.dir/ParseCache.txt"
  "tests/Tests_autogen"
  )
endif()
