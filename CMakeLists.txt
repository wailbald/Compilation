cmake_minimum_required(VERSION 3.10)
project(c_compiler)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
add_compile_options(-Wall -Wextra -Ofast -mavx2 -march=native -mtune=native -finline-functions -funroll-loops -ftree-loop-vectorize -ftree-vectorize)

set(SOURCE
   ${SOURCE}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.cpp
)

set(TEST_TOKEN_SRC
   ${TEST_TOKEN_SRC}
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

set(HEADERS
   ${HEADERS}
   ${CMAKE_CURRENT_SOURCE_DIR}/src/ast.hpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/lexer.hpp   
   ${CMAKE_CURRENT_SOURCE_DIR}/src/token.hpp
)

#add_executable(c_compiler ${SOURCE} ${HEADERS})

add_executable(test_token ${TEST_TOKEN_SRC} ${HEADERS})
add_executable(test_lexer ${TEST_LEXER_SRC} ${HEADERS})



