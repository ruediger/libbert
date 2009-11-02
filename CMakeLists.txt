# Based on the Flusspferd (http://flusspferd.org) CMakeLists.txt

# The MIT License
#
# Copyright (c) 2008, 2009 Flusspferd contributors (see "CONTRIBUTORS" 
#                                      http://flusspferd.org/contributors.txt)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#

project(Libbert)

cmake_minimum_required(VERSION 2.6)

set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
#set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)
if(NOT CMAKE_BUILD_TYPE)
    set(
        CMAKE_BUILD_TYPE
        Debug
        CACHE
        STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
        FORCE)
endif()

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(FATAL_ERROR "In-source builds are not permitted. Make a separate folder for building:\nmkdir build; cd build; cmake ..\nBefore that, remove the files that cmake just created:\nrm -rf CMakeCache.txt CMakeFiles")
endif()

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

set(INSTALL_EXECUTABLES_PATH ${CMAKE_INSTALL_PREFIX}/bin)
set(INSTALL_LIBRARIES_PATH ${CMAKE_INSTALL_PREFIX}/lib)
set(INSTALL_HEADERS_PATH ${CMAKE_INSTALL_PREFIX}/include)

if(NOT WIN32 AND NOT LIBBERT_VERSION)
    execute_process(
        COMMAND "${Libbert_SOURCE_DIR}/version.sh"
        WORKING_DIRECTORY "${Libbert_SOURCE_DIR}"
        OUTPUT_VARIABLE LIBBERT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()

if(NOT LIBBERT_VERSION)
    set(LIBBERT_VERSION unknown)
endif()

add_definitions("-DLIBBERT_VERSION=\"${LIBBERT_VERSION}\"")
add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")

message(STATUS "Libbert Version: " ${LIBBERT_VERSION})

include(CheckIncludeFile)
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckCXXSourceRuns)
include(FindPkgConfig)

option(
  ENABLE_TESTS
  "Compile the test suite"
  ON)

## Compiler ####################################################################

message(STATUS "Build Type: " ${CMAKE_BUILD_TYPE})
if(CMAKE_COMPILER_IS_GNUCXX)
  # MinGW doesn't set this by default
  if(WIN32)
    add_definitions(-DWIN32)
  endif()

  set(CMAKE_CXX_FLAGS
    " -pedantic-errors -Wall -Wextra -Winit-self -Woverloaded-virtual -Wnon-virtual-dtor -Wsign-promo -Wpadded -Weffc++ -Wno-long-long -std=c++98")

  set(CMAKE_C_FLAGS
    " -pedantic-errors -Wall -Wextra -std=c99")

  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG ")
  set(CMAKE_CXX_FLAGS_DEBUG "-g3 -DDEBUG ")
  set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG ")
  set(CMAKE_C_FLAGS_DEBUG "-g3 -DDEBUG ")
elseif(MSVC)
  add_definitions("/W3 /wd4512 /wd4355 /wd4800 /wd4996 /wd4714 /EHa")
  set(WINLIBS projects/windows/lib32)
  set(WININCLUDES ${CMAKE_SOURCE_DIR}/${WINLIBS})
endif()

include_directories(${Libbert_SOURCE_DIR}/include)

## Find Packages ###############################################################

#### Boost #####################################################################

set(Boost_FIND_REQUIRED TRUE)
if(ENABLE_TESTS)
  set(LIBBERT_BOOST_LIBRARIES unit_test_framework ${LIBBERT_BOOST_LIBRARIES})
endif()

find_package(Boost 1.36 COMPONENTS ${LIBBERT_BOOST_LIBRARIES})

if(Boost_INCLUDE_DIR)
  include_directories(${Boost_INCLUDE_DIR})
else()
  message(FATAL_ERROR "Boost headers not found. You will have to specify them in your build environment or you can use -DBOOST_ROOT or -DBOOST_INCLUDE_DIR to tell cmake where your boost installation is.")
endif()

if(Boost_LIBRARY_DIRS)
  link_directories(${Boost_LIBRARY_DIRS})
else()
  message(FATAL_ERROR "Boost libraries not found. Please set -DBOOST_ROOT or -DBOOST_LIBRARY_DIR manually.")
endif()

if(NOT Boost_FOUND)
  message(STATUS "Boost not found!")
endif()

## Library #####################################################################

### Libbert ####################################################################
file(
  GLOB
  libbert_library_sources
  ${Libbert_SOURCE_DIR}/src/*.c++
  ${Libbert_SOURCE_DIR}/src/*.cpp
  ${Libbert_SOURCE_DIR}/include/bert/*.hpp
  ${Libbert_SOURCE_DIR}/include/*.hpp
  ${Libbert_SOURCE_DIR}/include/*.h)

add_library(
  bert
  SHARED
  ${libbert_library_sources})

if(WIN32)
  install(TARGETS bert RUNTIME DESTINATION ${INSTALL_EXECUTABLES_PATH})
  install(TARGETS bert ARCHIVE DESTINATION ${INSTALL_LIBRARIES_PATH})
else()
  install(TARGETS bert LIBRARY DESTINATION ${INSTALL_LIBRARIES_PATH})
endif()

### C API ######################################################################
file(
  GLOB
  libbert_capi_library_sources
  ${Libbert_SOURCE_DIR}/src/capi/*.c++
  ${Libbert_SOURCE_DIR}/src/capi/*.cpp
  ${Libbert_SOURCE_DIR}/include/bert/*.hpp
  ${Libbert_SOURCE_DIR}/include/*.hpp
  ${Libbert_SOURCE_DIR}/include/*.h)

add_library(
  bert_capi
  SHARED
  ${libbert_capi_library_sources})
target_link_libraries(bert_capi bert)

if(WIN32)
    install(TARGETS bert_capi RUNTIME DESTINATION ${INSTALL_EXECUTABLES_PATH})
    install(TARGETS bert_capi ARCHIVE DESTINATION ${INSTALL_LIBRARIES_PATH})
else()
    install(TARGETS bert_capi LIBRARY DESTINATION ${INSTALL_LIBRARIES_PATH})
endif()

## Programs ####################################################################

## Unit Tests ##################################################################

if(ENABLE_TESTS)
  message(STATUS "Building Tests")

  add_executable(
    bert-test
    ${Libbert_SOURCE_DIR}/tests/bert.c++)

  add_executable(
    bert-capi-test
    ${Libbert_SOURCE_DIR}/tests/bert.c)
  target_link_libraries(
    bert-capi-test
    bert_capi)

  file(GLOB TESTS ${Libbert_SOURCE_DIR}/tests/test_*.c++)
  foreach(TEST_SOURCE ${TESTS})
    string(REGEX MATCH "test_[a-zA-Z0-9_+]*" TEST_OUTPUT ${TEST_SOURCE})
    string(REGEX REPLACE "^test_" "" TEST_NAME TEST_OUTPUT)
    add_executable(${TEST_OUTPUT} ${TEST_SOURCE})
    set_property(
      TARGET ${TEST_OUTPUT}
      PROPERTY COMPILE_FLAGS
      "-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MODULE=${TEST_NAME}")
    target_link_libraries(
            ${TEST_OUTPUT}
            bert ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
  endforeach()

  ## TODO .c tests
endif()

## Headers #####################################################################

file(GLOB_RECURSE HEADERFILES *.hpp) # TODO .h Headers

foreach(HEADER ${HEADERFILES})
  set_source_files_properties(${HEADER}
    PROPERTIES
    HEADER_FILE_ONLY TRUE)
endforeach()

install(
    DIRECTORY include/
    DESTINATION ${INSTALL_HEADERS_PATH}
    FILES_MATCHING PATTERN "*.hpp")

install(
    DIRECTORY include/bert/
    DESTINATION ${INSTALL_HEADERS_PATH}/bert
    FILES_MATCHING PATTERN "*.hpp")

install(
    DIRECTORY include/
    DESTINATION ${INSTALL_HEADERS_PATH}
    FILES_MATCHING PATTERN "*.h")

## Documentation ############################################################

if(NOT MSVC)
  set(LIBBERT_EXTERNAL_CFLAGS "-I${Boost_INCLUDE_DIR}")

  configure_file(
    ${Libbert_SOURCE_DIR}/libbert.pc.in
    ${Libbert_BINARY_DIR}/libbert.pc
    @ONLY)
  install(
    FILES ${Libbert_BINARY_DIR}/libbert.pc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

  configure_file(
    ${Libbert_SOURCE_DIR}/libbert_capi.pc.in
    ${Libbert_BINARY_DIR}/libbert_capi.pc
    @ONLY)
  install(
    FILES ${Libbert_BINARY_DIR}/libbert_capi.pc
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
endif()

message(STATUS "Configuration SUCCESSFUL. No fatal errors.")
