cmake_minimum_required(VERSION 2.8)	 # Проверка версии CMake.
										# Если версия установленой программы
										# старее указаной, произайдёт аварийный выход.

project(hello_world)			# Название проекта

set(SOURCE_EXE main.c)		# Установка переменной со списком исходников для исполняемого файла

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
	message(SEND_ERROR "Failed to find SDL")
	return()
else()
	include_directories(${SDL2_INCLUDE_DIRS})
endif()

find_package(SDL2_image REQUIRED)
if(NOT SDL2_IMAGE_FOUND)
	message(SEND_ERROR "Failed to find SDL_image")
	return()
else()
	include_directories(${DL2_IMAGE_INCLUDE_DIRS})
endif()

target_link_libraries(main 
								${SDL2_LIBRARIES}
								${SDL2_IMAGE_LIBRARIES})