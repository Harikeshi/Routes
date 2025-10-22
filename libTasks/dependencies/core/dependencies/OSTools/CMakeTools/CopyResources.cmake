# Функция для копирования ресурсов в каталог сборки для Windows(MSVC) и Linux.
# Обертка над функцией file(COPY).
# Примечание:
# Перед использованием необходимо установить переменную
# CMAKE_RUNTIME_OUTPUT_DIRECTORY
# Аргументы:
# source — папка источник.
#-------------------------------------------------------------------------------
function(copy_resources source)
  if("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" STREQUAL "")
    set(CURRENT_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  else()
    set(CURRENT_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endif()

  if(MSVC)
    file(COPY "${source}"
         DESTINATION "${CURRENT_RUNTIME_OUTPUT_DIRECTORY}/Debug")
    file(COPY "${source}"
         DESTINATION "${CURRENT_RUNTIME_OUTPUT_DIRECTORY}/Release")
  else()
    file(COPY "${source}"
         DESTINATION "${CURRENT_RUNTIME_OUTPUT_DIRECTORY}")
  endif()

  file(COPY "${source}"
       DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endfunction()
#-------------------------------------------------------------------------------