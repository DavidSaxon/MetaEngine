cmake_minimum_required(VERSION 2.6)
project(MetaEngine)

set(LIB_SRC
	src/cpp/json/jsoncpp.cpp

	src/cpp/metaeng/Data.cpp
)

set(TESTS_SUITES
    tests/cpp/TestsMain.cpp

    tests/cpp/Data_TestSuite.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -Wall -Wno-varargs")

include_directories(
	${INCLUDE_DIRECTORIES}
	${CMAKE_BINARY_DIR}/ext/ArcaneCore/include/
	src/cpp/
	tests/cpp/
)

link_directories(
	${LINK_DIRECTORIES}
	${CMAKE_BINARY_DIR}/ext/ArcaneCore/lib/linux_x86
	${CMAKE_BINARY_DIR}/build/linux_x86
)

add_library(metaengine SHARED ${LIB_SRC})

target_link_libraries(metaengine
    arcanecore_base
    arcanecore_io
)

add_executable(tests ${TESTS_SUITES})

target_link_libraries(tests
	arcanecore_test
	arcanecore_base
	arcanecore_io
    metaengine
)
