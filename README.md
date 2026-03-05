[![CMake on multiple platforms](https://github.com/lukemcelya/notes-cli/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/lukemcelya/notes-cli/actions/workflows/cmake-multi-platform.yml)

# Notes CLI

A modular command-line note-taking application written in modern C++.

This project demonstrates clean separation between CLI parsing and domain logic,
use of 'std::optional', move semantics, and structured command handling.

---

## Build Instructions

### Requirements

- Cmake 3.16+
- C++20 compatible compiler

### Build

```bash
git clone https://github.com/lukemcelya/cli-notes.git
cd cli-notes

mkdir build
cd build

cmake ..
cmake --build .
```

## Usage

Run directly with arguments:

```bash
./notes list
./notes add --title "My Title" --body "Text"
./notes edit 1 --title "Updated Title"
./notes delete 1
./notes view 1
```

Or launch interactive mode:
```bash
./notes
```

Then use commands
```Code
list
add --title My Title --body Text
edit 1 --title Updated
delete 1
exit
```

## Features
- Add notes
- Edit notes
- Delete notes
- View individual notes
- List all notes
- Interactive REPL mode
- Save and load from JSON

---


## Architecture

The project is structured with a layered design:

- `NotesApp` - Core domain logic and note management
- `Cli` - Command parsing, validation, and user interaction
- `JsonNoteStore` - JSON parsing, save, load methods
- File-local parsing helpers isolated in anonymous namespaces

## Roadmap
* [x] JSON file persistence
* [ ] SQLite storage backend
* [ ] Search functionality
* [ ] Unit tests
