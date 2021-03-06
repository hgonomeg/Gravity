cmake_minimum_required(VERSION 3.1)
enable_language(CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

find_package(cpptoml 0.4.0 REQUIRED)

project(Gravity)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(SFML_STATIC "Is SFML built as a static library?" FALSE)

set(SFML_STATIC_LIBRARIES ${SFML_STATIC})

option(GRAVITY_STATIC "Do you want the final executable to be statically linked?" FALSE)

if(WIN32)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif()

include_directories(${SFML_DIR}/include)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})

option(BUILD_WUXING "Should the Wuxing subproject be built?" FALSE)

file(GLOB LIST_OF_CPP ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(Gravity ${LIST_OF_CPP})

if(GRAVITY_STATIC) 
	if(MSVC)
		add_link_options(/MT)
		add_definitions(/DSFML_STATIC)
	else()
		set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} -static)
		add_definitions(-DSFML_STATIC)
	endif()
endif()

if(GCC)
	target_link_libraries(Gravity stdc++fs)
endif()

target_link_libraries(Gravity sfml-graphics Threads::Threads)

install(TARGETS Gravity DESTINATION bin)

if(BUILD_WUXING)
	file(GLOB WUXING_CPP ${PROJECT_SOURCE_DIR}/pairalgo/*.cpp)
	include_directories(${PROJECT_SOURCE_DIR}/pairalgo)
	add_executable(Wuxing ${WUXING_CPP})
	target_link_libraries(Wuxing sfml-graphics Threads::Threads)
endif()