cmake_minimum_required(VERSION 4.1)
project(CppPrep)

set(CMAKE_CXX_STANDARD 20)

add_executable(notes
        src/main.cpp
        src/NotesApp.cpp
        src/NotesApp.h
        src/Cli.cpp
        src/Cli.h)
