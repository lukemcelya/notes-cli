cmake_minimum_required(VERSION 3.24)
project(NotesApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Core library
add_library(notes_core
        src/app/NotesApp.cpp
        src/cli/Cli.cpp
        src/storage/JsonNoteStore.cpp
)

target_include_directories(notes_core PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external
)

# Main exe
add_executable(notes
        src/main.cpp
)

target_link_libraries(notes PRIVATE notes_core)


# Tests
enable_testing()
add_subdirectory(tests)