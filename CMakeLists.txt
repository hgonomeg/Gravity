cmake_minimum_required(VERSION 3.1)
enable_language(CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
project(Gravity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(SFML_STATIC "Is SFML built as a static library?" TRUE)

set(SFML_STATIC_LIBRARIES ${SFML_STATIC})

option(GRAVITY_STATIC "Do you want the final executable to be statically linked?" TRUE)

if(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

include_directories(${SFML_DIR}/include)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

file(GLOB LIST_OF_CPP ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(Gravity ${LIST_OF_CPP})

if(GRAVITY_STATIC) 
	if(MSVC)
		add_link_options(/MT)
		add_definitions(/DSFML_STATIC)
	else()
		add_link_options(-static)
		add_definitions(-DSFML_STATIC)
	endif()
endif()

target_link_libraries(Gravity sfml-graphics Threads::Threads)