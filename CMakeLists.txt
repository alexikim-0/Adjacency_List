cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -pedantic-errors -Werror -ggdb -fsanitize=address")
set(CMAKE_EXE_LINKER_FLAGS "-fsanitize=address")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(skiplist)


file(GLOB TESTS ${CMAKE_SOURCE_DIR}/tests/public/*.cpp ${CMAKE_SOURCE_DIR}/tests/custom/custom.cpp)
file(GLOB TEST_SETUP ${CMAKE_SOURCE_DIR}/tests/setup/*.cpp)
file(GLOB SRC ${CMAKE_SOURCE_DIR}/src/*.cpp)

add_executable(all-tests
    ${TESTS}
    ${TEST_SETUP}
    ${SRC}
    )

target_include_directories(all-tests PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests)

function(make_test_targets_from_files tests depends)
    foreach(file_path ${tests})
        get_filename_component(filename "${file_path}" NAME_WLE)
        add_executable("${filename}" 
        "${file_path}"
        ${TEST_SETUP}
        ${depends}
        )
        target_include_directories("${filename}" PRIVATE ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests)

    endforeach()
endfunction()


make_test_targets_from_files("${TESTS}" "${SRC}")

