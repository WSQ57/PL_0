cmake_minimum_required(VERSION 3.21)
project(PL_0)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s")
add_executable(PL_0 main.cpp LEX.cpp LEX.h AST.h AST.cpp SyntaxAnalysis.cpp SyntaxAnalysis.h SymbolTable.cpp SymbolTable.h handleError.h targetIns.h interpreter.cpp interpreter.h)
