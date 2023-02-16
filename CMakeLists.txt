cmake_minimum_required(VERSION 3.16)
project(huffman_Quillo123)

set(CMAKE_CXX_STANDARD 14)

add_executable(huffman main.cpp HuffTree.h HuffTree.cpp KeyTable.h KeyTable.cpp EncWriter.h EncWriter.cpp EncReader.h EncReader.cpp )