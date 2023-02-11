cmake_minimum_required(VERSION 3.10)
project(c_compiler)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
add_compile_options(-Wall -Wextra -Ofast -mavx2 -march=native -mtune=native -finline-functions -funroll-loops -ftree-vectorize)

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/binder.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/dumper.cpp
)

set(TEST_TOKEN_SRC
   ${TEST_TOKEN_SRC}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/test_token.cpp
)

set(TEST_LEXER_SRC
   ${TEST_LEXER_SRC}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/test_lexer.cpp
)

set(TEST_PARSER_SRC
   ${TEST_PARSER_SRC}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/binder.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/dumper.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/test_parser.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/type_checker.cpp
)

set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast_regex.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/binder.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/dumper.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.hpp   
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/type_checker.hpp
)

#add_executable(c_compiler ${SOURCE} ${HEADERS})

add_executable(test_token ${TEST_TOKEN_SRC} ${HEADERS})
add_executable(test_lexer ${TEST_LEXER_SRC} ${HEADERS})
add_executable(test_parser ${TEST_PARSER_SRC} ${HEADERS})



