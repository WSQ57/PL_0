cmake_minimum_required(VERSION 3.21)
project(PL_0)

set(CMAKE_CXX_STANDARD 11)

add_executable(PL_0 main.cpp LEX.cpp LEX.h AST.h AST.cpp SyntaxAnalysis.cpp SyntaxAnalysis.h SymbolTable.cpp SymbolTable.h handleError.h)
