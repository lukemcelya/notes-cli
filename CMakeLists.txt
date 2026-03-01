cmake_minimum_required(VERSION 4.1)
project(NotesApp)

set(CMAKE_CXX_STANDARD 20)

add_executable(notes
        src/main.cpp
        src/NotesApp.cpp
        src/NotesApp.h
        src/Cli.cpp
        src/Cli.h
        src/model/Note.h)

target_include_directories(notes PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
