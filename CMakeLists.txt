cmake_minimum_required(VERSION "3.21")

project("console-engine")

set(PROJECT_CXX_STANDARD cxx_std_20)

# Selects for all following targets a multi-threaded statically-linked
# runtime library with or without debug information depending on the configuration.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(
	HEADERS
	"character.hpp"
	"console.hpp"
	"cursor_info.hpp"
	"exception.hpp"
	"font_info_ex.hpp"
	"screen_buffer_size.hpp"
	"win_utils.hpp"
	"windows.hpp"
)

set(
	SOURCES
	"character.cpp"
	"console.cpp"
	"win_utils.cpp"
	"main.cpp"
)

set(HEADER_DIR "include/")
set(SOURCE_DIR "src/")

list(TRANSFORM HEADERS PREPEND ${HEADER_DIR})
list(TRANSFORM SOURCES PREPEND ${SOURCE_DIR})

add_executable(
	${PROJECT_NAME} ${SOURCES} ${HEADERS}
)

# set project's C++ standard
target_compile_features(
	${PROJECT_NAME} PRIVATE ${PROJECT_CXX_STANDARD}
)

target_include_directories(
	${PROJECT_NAME} PRIVATE ${HEADER_DIR}
)
target_sources(
	${PROJECT_NAME} PRIVATE ${SOURCES} ${HEADERS}
)

# dependencies
find_package(fmt CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt::fmt)

#[[ # uncomment this block to enable google test

# google test section
set(TEST_SUBPROJECT_NAME "unit-tests")

include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
if (MSVC)
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_DIR "test/")

set(
	TEST_SOURCES
	"test.cpp"
)

list(TRANSFORM TEST_SOURCES PREPEND ${TEST_DIR})

add_executable(
	${TEST_SUBPROJECT_NAME} ${TEST_SOURCES}
)

# set test project's C++ standard
target_compile_features(
	${TEST_SUBPROJECT_NAME} PRIVATE ${PROJECT_CXX_STANDARD}
)

target_link_libraries(
	${TEST_SUBPROJECT_NAME}
	gtest_main
)

target_include_directories(
	${TEST_SUBPROJECT_NAME} PRIVATE ${HEADER_DIR}
)

include(GoogleTest)
gtest_discover_tests(${TEST_SUBPROJECT_NAME})
]]

# man I love warnings! please send more

# cl.exe or clang-cl.exe
if(MSVC OR(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC"))
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
		"/permissive-"
		"/W4" "/w14640"
		"/w14242" "/w14254" "/w14263" "/w14265"
		"/w14287" "/we4289" "/w14296" "/w14311"
		"/w14545" "/w14546" "/w14547" "/w14549"
		"/w14555" "/w14619" "/w14826" "/w14905"
		"/w14906" "/w14928"
	)
endif()

# g++ or clang++
if((CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU") OR MINGW)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
		"-Wall" "-Wextra" "-Wshadow" "-pedantic"
		"-Wnon-virtual-dtor" "-Wunused"
		"-Wconversion" "-Wsign-conversion"
		"-Wnull-dereference" "-Wdouble-promotion"
		"-Woverloaded-virtual" "-Wpedantic"
		"-Wcast-align" "-Wformat=2"
	)
endif()

# g++ exclusive flags
if(MINGW)
	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
		"-Wmisleading-indentation" "-Wduplicated-cond"
		"-Wduplicated-branches" "-Wlogical-op"
		"-Wnull-dereference" "-Wuseless-cast"
	)
endif()
