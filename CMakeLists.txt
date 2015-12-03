cmake_minimum_required (VERSION 2.8)
project(ticTacToe)

if (CMAKE_COMPILER_IS_GNUCXX )
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

set(SOURCES_CPP main.cpp)

source_group("src" FILES ${SOURCES_CPP})

add_executable(${CMAKE_PROJECT_NAME}
	${SOURCES_CPP}
	)
