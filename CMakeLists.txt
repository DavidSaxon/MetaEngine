cmake_minimum_required(VERSION 2.6)
project(ChaosCore)

set(LIB_SRC
	ext/jsoncpp/jsoncpp.cpp

	src/cxx/meta/Accessor.cpp
)

set(TESTS_SUITES
    tests/cxx/TestsMain.cpp

    tests/cxx/Accessor_TestSuite.cpp
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -Wall -Wno-varargs")

include_directories(
	${INCLUDE_DIRECTORIES}
	${CMAKE_BINARY_DIR}/ext/ChaosCore/include/
	${CMAKE_BINARY_DIR}/ext/jsoncpp/
	src/cxx/
	tests/cxx/
)

link_directories(
	${LINK_DIRECTORIES}
	${CMAKE_BINARY_DIR}/ext/ChaosCore/lib/linux_x86
	${CMAKE_BINARY_DIR}/build/linux_x86
)

add_library(metaengine SHARED ${LIB_SRC})

target_link_libraries(metaengine
    chaoscore_base
    chaoscore_io
)

add_executable(tests ${TESTS_SUITES})

target_link_libraries(tests
	chaoscore_test
	chaoscore_base
	chaoscore_io
    metaengine
)
