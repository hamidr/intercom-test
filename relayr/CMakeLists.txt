cmake_minimum_required (VERSION 2.8)

project (ableton)

set(DEBUG_MODE ON CACHE BOOL "Debug mode") 
set(LIB_INSTALL_DIR "/ust/local/lib/" CACHE PATH "")

if(NOT DEBUG_MODE)
  ##SET RELEASE MODE FLAGS
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )
else()
  ##SET DEUBG MODE FLAGS
  SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -g" )
  SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS} -g" )
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(PROJECT_SOURCE_DIR src)
set(PROJECT_INCLUDE_DIR includes)

include_directories(${PROJECT_INCLUDE_DIR})
include_directories("/usr/include/")
include_directories("/usr/local/include")

link_directories("/usr/lib")
link_directories("/usr/local/lib")


set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tests/main_test.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)
