project(lab03)
# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be 11 or 14
set(CMAKE_CXX_STANDARD 20)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)
# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
set(ANTLR_EXECUTABLE /usr/local/lib/antlr-4.9.3-complete.jar)
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(FormatParser Format.g4 LEXER PARSER
        PACKAGE antlrcpptest
        COMPILE_FLAGS)

# include generated files in project environment
include_directories(${ANTLR_FormatParser_OUTPUT_DIR})


configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif ()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)
if (result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif ()

add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL
)

# add generated grammar to demo binary target
add_executable(gen gen.cpp
        ${ANTLR_FormatLexer_CXX_OUTPUTS}
        ${ANTLR_FormatParser_CXX_OUTPUTS} src/grammar/grammar.cpp src/grammar/grammar.h src/grammar/rule.cpp src/grammar/rule.h src/grammar/term.cpp src/grammar/term.h src/grammar/nterm.cpp src/grammar/nterm.h src/generator/gen_parser.cpp src/generator/gen_parser.h src/generator/gen_lexer.cpp src/generator/gen_lexer.h src/generator/util.cpp src/generator/util.h)
target_link_libraries(gen antlr4_static gtest_main)

# calculator
add_executable(run-calc
        generator-output/calc/main.cpp
        generator-output/calc/pretty_parser_calc.cpp
        generator-output/calc/pretty_lexer_calc.cpp)
add_executable(test-calc
        generator-output/calc/test.cpp
        generator-output/calc/pretty_parser_calc.cpp
        generator-output/calc/pretty_lexer_calc.cpp)
target_link_libraries(test-calc gtest_main)


# variables
add_executable(run-variables-c
        generator-output/variables-c/main.cpp
        generator-output/variables-c/pretty_parser_variables_c.cpp
        generator-output/variables-c/pretty_lexer_variables_c.cpp)
add_executable(test-variables-c
        generator-output/variables-c/test.cpp
        generator-output/variables-c/pretty_parser_variables_c.cpp
        generator-output/variables-c/pretty_lexer_variables_c.cpp)
target_link_libraries(test-variables-c gtest_main)