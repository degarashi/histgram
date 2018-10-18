cmake_minimum_required(VERSION 2.8)
project(histgram)

set(CXX_VERSION 17)
include(lubee/cxx_options.cmake)
string(CONCAT CMAKE_CXX_FLAGS " -Wno-gnu-anonymous-struct")

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)
aux_source_directory(src LIBSRC)
add_library(histgram SHARED ${LIBSRC})
set_target_properties(histgram PROPERTIES VERSION 0.0.1 SOVERSION 0)

if(NOT without-unittest)
	enable_testing()
	aux_source_directory(src/tests TESTSRC)
	add_executable(histgram_test ${TESTSRC})

	find_package(Threads REQUIRED)
	find_package(GTest REQUIRED)
	include_directories(
		${GTEST_INCLUDE_DIRS}
		src
	)
	target_link_libraries(histgram_test
		histgram
		${GTEST_LIBRARIES}
		${GTEST_MAIN_LIBRARIES}
		${CMAKE_THREAD_LIBS_INIT}
	)
	add_test(
		NAME histgram_test
		COMMAND $<TARGET_FILE:histgram_test>
	)
endif()
