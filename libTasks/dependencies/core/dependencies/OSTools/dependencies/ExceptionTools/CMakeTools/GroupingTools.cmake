#===============================================================================
# Функция для группирования файлов по пути их расположения.
# (актуально для  VisualStudio)
#
# group_files_by_path(
#   [ROOT_PATH dirPath]   - (опционально) корневой путь, относительно которого 
#                           необходимо произвести группирование файлов. 
#                           Если не задан, то группирование выполняется 
#                           отосительно ${CMAKE_CURRENT_SOURCE_DIR}.
#
#   [ROOT_GROUP groupName] - (опционально) корневая группа. Добавляется в корень 
#                            пути группирования. 
#
#   [SUB_GROUP groupName] -  (опционально) подгруппа. Добавляется в конец 
#                            пути группирования. 
#
#   [DEFAULT_GROUP groupName] - (опционально) группа по умолчанию.
#                               Используется в случае если сгенерированное 
#                               имя группы пустое.
#   
#   [FILES filePath1 [FilePath2 ...]] - пути файлов.
#
#   [TARGET targetName] -  имя цели, файлы которой должны быть сгруппированы
#                         (по умолчанию, если не указаны ни цель ни файлы, будет
#                          выполнен поиск исходников у цели ${PROJECT_NAME})
#   ) 

function(group_files_by_path)

	# Парсинг аргументов ->
	set(options )
	set(oneValueParams ROOT_PATH ROOT_GROUP SUB_GROUP DEFAULT_GROUP TARGET)
	set(multiValueParams FILES)	

	cmake_parse_arguments(PARAM "${options}"
		"${oneValueParams}" "${multiValueParams}" ${ARGN})
	# <- Парсинг аргументов

	# Проверка аргуметов ->
	if (PARAM_UNPARSED_ARGUMENTS)
		message("group_files_by_path() :: Warning :: Unparsed arguments: \"${PARAM_UNPARSED_ARGUMENTS}\"")
	endif()
    
    set(FILE_LIST)   
	
	if(PARAM_FILES)
        set(FILE_LIST ${FILE_LIST} ${PARAM_FILES})
    elseif(NOT PARAM_TARGET)
        if(TARGET "${PROJECT_NAME}")
            set(PARAM_TARGET ${PROJECT_NAME})
        endif()        
    endif()
    
    if(PARAM_TARGET)    
        if(TARGET "${PARAM_TARGET}")
            get_target_property(tfiles ${PARAM_TARGET} SOURCES)
            set(FILE_LIST ${FILE_LIST} ${tfiles})
        else()
           message("[${PROJECT_NAME}] group_files_by_path() :: Warning :: Non-existent target: \"${PARAM_TARGET}\"")
        endif()
    endif()

	set(ROOT_PATH)	
	if(PARAM_ROOT_PATH)
		set(ROOT_PATH ${PARAM_ROOT_PATH})		
	else()		
		set(ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
 
    
    if(NOT FILE_LIST)
        message("[${PROJECT_NAME}] group_files_by_path() :: Warning :: No files")
        return()
    endif()    
	# <- Проверка аргуметов 
	
	# Группирование ->    
	foreach(f ${FILE_LIST})
		get_filename_component(DIR ${f} PATH)        
        if(NOT DIR)                    
            set(DIR ".")
        endif()
         
        file(GLOB RPATH RELATIVE ${ROOT_PATH} ${DIR})
        set(groop)
        if(RPATH)
            string(REGEX REPLACE "/" "\\\\" sg ${RPATH})			
            set(groop ${sg})
            if(PARAM_ROOT_GROUP)
                if(groop)
                    set(groop ${PARAM_ROOT_GROUP}\\${groop})
                else()
                    set(groop ${PARAM_ROOT_GROUP})
                endif()                
            endif()
            if(PARAM_SUB_GROUP)
                if(groop)
                    set(groop ${groop}\\${PARAM_SUB_GROUP})
                else()
                    set(groop ${PARAM_SUB_GROUP})
                endif()
            endif()
        endif()
        
        if(groop)			
            source_group(${groop} FILES ${f})
        elseif(PARAM_DEFAULT_GROUP)
            source_group(${PARAM_DEFAULT_GROUP} FILES ${f})
        endif()
	endforeach()
	# <- Группирование
	
endfunction()
#===============================================================================
# Функция для группирования целей(проектов) по пути их расположения.
# (актуально для  VisualStudio)
#
# group_targets_by_path(
#   [ROOT_PATH dirPath]   - (опционально) корневой путь, относительно которого 
#                           необходимо произвести группирование целей. 
#                           Если не задан, то группирование выполняется 
#                           отосительно ${CMAKE_SOURCE_DIR}.
#
#   [ROOT_GROUP groupName] - (опционально) корневая группа. Добавляется в корень 
#                            пути группирования. 
#
#   [SUB_GROUP groupName] -  (опционально) подгруппа. Добавляется в конец 
#                            пути группирования. 
#
#   [DEFAULT_GROUP groupName] - (опционально) группа по умолчанию.
#                               Используется в случае если сгенерированное 
#                               имя группы пустое.
#   
#   [PROJECT_NAME projectName] - (опционално) имя проекта. 
#
#   [PROJECT_SOURCE_DIR projectDir] - (опционално) путь к проекту, по умолчанию
#                                   используется ${${PROJECT_NAME}_SOURCE_DIR}.
#
#   [TARGETS targetName] - имена целей, которые надо формировать. Если не задано
#                          будет использовано имя ${PROJECT_NAME} как имя цели.
#                          
#   ) 

function(group_targets_by_path) # in_projects in_rootFolder

    # Парсинг аргументов ->
	set(options)
	set(oneValueParams  ROOT_PATH ROOT_GROUP SUB_GROUP DEFAULT_GROUP
                        PROJECT_SOURCE_DIR PROJECT_NAME)
	set(multiValueParams TARGETS)

	cmake_parse_arguments(PARAM "${options}"
		"${oneValueParams}" "${multiValueParams}" ${ARGN})
	# <- Парсинг аргументов
    
    
    # Проверка аргуметов ->
	if (PARAM_UNPARSED_ARGUMENTS)
		message("group_targets_by_path() :: Warning :: Unparsed arguments: \"${PARAM_UNPARSED_ARGUMENTS}\"")
	endif()
    
    set(ROOT_PATH)
    if(PARAM_ROOT_PATH)
        set(ROOT_PATH ${PARAM_ROOT_PATH})
    else()
        set(ROOT_PATH ${CMAKE_SOURCE_DIR})
    endif()    
    
    set(ROOT_GROUP ${PARAM_ROOT_GROUP})
    set(SUB_GROUP ${PARAM_SUB_GROUP})
    
	
    set(PROJECT_SOURCE_DIR ${PARAM_PROJECT_SOURCE_DIR})
    
    set(DEFAULT_GROUP ${PARAM_DEFAULT_GROUP})
    
  
    if(PARAM_PROJECT_NAME)
        set(PROJECT_NAME ${PARAM_PROJECT_NAME})    
    endif()
	
    
    if(NOT PROJECT_SOURCE_DIR)
        set(PROJECT_SOURCE_DIR ${${PROJECT_NAME}_SOURCE_DIR})
    endif()
    

    set(TARGETS ${PARAM_TARGETS})
    if(NOT TARGETS)
        if(TARGET "${PROJECT_NAME}")
            set(TARGETS ${PROJECT_NAME})
        else()
            message("group_targets_by_path() :: Warning :: Target list is empty")
            return()
        endif()
    endif()    
    # <- Проверка аргуметов
    
    
    # Группирование ->    
    get_filename_component(SOURCE_DIR ${PROJECT_SOURCE_DIR} PATH)    
    if(NOT SOURCE_DIR)                    
        set(SOURCE_DIR ".")
    endif()
	
         
    file(GLOB RPATH RELATIVE ${ROOT_PATH} ${SOURCE_DIR})
    
    if(RPATH)
        set(GROUP ${RPATH})
    elseif(DEFAULT_GROUP)
        set(GROUP ${DEFAULT_GROUP})
    endif()
    
    if(ROOT_GROUP)
        if(GROUP)
            set(GROUP "${ROOT_GROUP}/${GROUP}")
        else()
            set(GROUP "${ROOT_GROUP}")
        endif()
    endif()

    if(SUB_GROUP)
        if(GROUP)
            set(GROUP "${GROUP}/${SUB_GROUP}")
        else()
            set(GROUP "${SUB_GROUP}")
        endif()
    endif()
    
    foreach(t ${TARGETS})
        if(TARGET "${t}")
            set_property(TARGET ${t} PROPERTY FOLDER "${GROUP}")
        else()
            message("group_targets_by_path() :: Warning :: Target \"${t}\" not exist")
        endif()
    endforeach()
    # <- Группирование   	
endfunction()
