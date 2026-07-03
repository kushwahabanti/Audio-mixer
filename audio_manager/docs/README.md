# Audio Manager (C++ CLI Application)

## Overview
Audio Manager is a command-line interface application built in C++17 that allows users to manage a sequence of audio edits. It implements an undo/redo system using a doubly linked list data structure. The application tracks audio edits including actions, categories, values, and timestamps, and supports features like jumping to specific versions, branching after undo, searching, and saving/loading projects.

## Features
- **Add Edit**: Add a new audio edit operation.
- **Undo/Redo**: Navigate backward and forward in the edit history.
- **Branching**: Adding a new edit after an undo will safely delete the forward history (creating a new branch).
- **History Tracking**: View full history, compare versions, and see edit metadata.
- **Save/Load**: Persist project state to file with an easily parseable pipe-delimited format.
- **Auto-Save**: The application autosaves when exiting.
- **Recent Projects**: Displays a list of recently opened projects.
- **Memory Safe**: The undo/redo system is built securely avoiding memory leaks.

## Directory Structure
```
audio_manager/
├── include/
│   ├── AudioEditNode.h
│   ├── DoublyLinkedList.h
│   ├── UndoRedoManager.h
│   ├── FileManager.h
│   └── Dashboard.h
├── src/
│   ├── AudioEditNode.cpp
│   ├── DoublyLinkedList.cpp
│   ├── UndoRedoManager.cpp
│   ├── FileManager.cpp
│   └── Dashboard.cpp
├── data/
│   └── recent_projects.txt
├── docs/
│   └── README.md
├── main.cpp
└── Makefile
```

## Building and Running

### Using Make
```bash
make
./audio_manager
```

### Manual Compilation
```bash
g++ -std=c++17 -Wall src/*.cpp main.cpp -I include -o audio_manager
./audio_manager
```

## Technical Details
- **Data Structure**: `DoublyLinkedList` handles the insertion, traversal, and deletion of `AudioEditNode` objects.
- **State Management**: `UndoRedoManager` wraps the list and uses a pointer (`current`) to track the current active state in the history.
- **Persistence**: `FileManager` stores data locally in text files.
- **UI Rendering**: `Dashboard` class keeps console prints isolated from business logic.
