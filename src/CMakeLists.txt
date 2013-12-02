# Compiling instructions for CMake
# Copyright (C) 2013 Matheus Caldas Santos
# This file is part of RNet 0.3

# RNet 0.3 is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# RNet 0.3 is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 2.8)

project(netmodel)

set (RNET_VERSION_MAJOR 1)
set (RNET_VERSION_MINOR 1)

set (CMAKE_BUILD_TYPE RELEASE)

add_definitions(-O3)
#add_definitions(-O0)
add_definitions(-ggdb)
add_definitions(-Wall)
add_definitions(-std=c++0x)

include_directories ("${PROJECT_SOURCE_DIR}/../include/")
include_directories("${PROJECT_BINARY_DIR}")

#find_package(MPI REQUIRED)
#set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
#set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
#include_directories(MPI_INCLUDE_PATH)
#include_directories("/usr/include/openmpi/")
#set (EXTRA_LIBS ${EXTRA_LIBS} ${MPI_LIBRARIES})

#GPROF!!!
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

find_package(OpenMP)
if(OPENMP_FOUND)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif(OPENMP_FOUND)

#FIND_PACKAGE( Boost COMPONENTS program_options REQUIRED )
#INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
#set (EXTRA_LIBS ${EXTRA_LIBS} ${Boost_LIBRARIES})

#include_directories ("${PROJECT_SOURCE_DIR}/libfastGraph")
#add_subdirectory (libfastGraph) 
#set (EXTRA_LIBS ${EXTRA_LIBS} fastGraph)

# DEBUG - Electric fence
#set (EXTRA_LIBS ${EXTRA_LIBS} efence)

configure_file (
	"${PROJECT_SOURCE_DIR}/CMakeConfig.h.in"
	"${PROJECT_BINARY_DIR}/CMakeConfig.h"
	    )

add_executable(rnet rnet.cpp config.cpp utils.cpp logo.cpp)

target_link_libraries (rnet 	${EXTRA_LIBS})
