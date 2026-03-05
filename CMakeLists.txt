cmake_minimum_required(VERSION 3.24)
project(NotesApp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(notes
        src/main.cpp
        src/app/NotesApp.cpp
        src/app/NotesApp.h
        src/cli/Cli.cpp
        src/cli/Cli.h
        src/model/Note.h
        src/storage/NotesState.h
        src/storage/JsonNoteStore.h
        src/storage/JsonNoteStore.cpp)

target_include_directories(notes PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/external)
