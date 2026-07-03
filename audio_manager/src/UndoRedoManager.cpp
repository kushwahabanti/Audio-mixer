#include "UndoRedoManager.h"

UndoRedoManager::UndoRedoManager() : current(nullptr), nextId(1), currentVersion(1) {}

UndoRedoManager::~UndoRedoManager() {}

void UndoRedoManager::branchAfterUndo() {
    if (current && current->next) {
        std::cout << "Branching detected: Deleting future history past current version (V" << current->version << ").\n";
        editHistory.deleteAfter(current);
    } else if (!current && editHistory.head) {
        std::cout << "Branching detected from empty state: Clearing all existing history.\n";
        editHistory.clearAll();
    }
}

void UndoRedoManager::addEdit(const std::string& action, const std::string& category, const std::string& value) {
    // If we're adding a new edit but not at the end of history, we need to branch.
    branchAfterUndo();

    AudioEditNode* newNode = new AudioEditNode(nextId++, currentVersion++, action, category, value);
    editHistory.insertAtEnd(newNode);
    current = newNode;
    std::cout << "Edit added successfully.\n";
}

bool UndoRedoManager::undo() {
    if (current == nullptr) {
        std::cout << "Nothing to undo. Currently at initial state.\n";
        return false;
    }
    
    current = current->prev;
    
    std::cout << "Undo successful. ";
    if (current) {
        std::cout << "Reverted to version V" << current->version << ".\n";
    } else {
        std::cout << "Reverted to initial empty state.\n";
    }
    return true;
}

bool UndoRedoManager::redo() {
    // Check if we can redo
    if (current == nullptr) {
        if (editHistory.head != nullptr) {
            current = editHistory.head;
            std::cout << "Redo successful. Advanced to version V" << current->version << ".\n";
            return true;
        } else {
            std::cout << "Nothing to redo. History is empty.\n";
            return false;
        }
    }
    
    if (current->next != nullptr) {
        current = current->next;
        std::cout << "Redo successful. Advanced to version V" << current->version << ".\n";
        return true;
    }
    
    std::cout << "Nothing to redo. Already at the latest version.\n";
    return false;
}

void UndoRedoManager::getCurrentVersion() const {
    if (current == nullptr) {
        std::cout << "Current State: Initial (Empty)\n";
    } else {
        std::cout << "Current State:\n";
        current->display();
    }
}

void UndoRedoManager::getHistory() const {
    std::cout << "\n--- Full Edit History ---\n";
    AudioEditNode* curr = editHistory.head;
    if (!curr) {
        std::cout << "History is empty.\n";
        return;
    }
    
    while (curr) {
        if (curr == current) {
            std::cout << "-> "; // Mark the current version
        } else {
            std::cout << "   ";
        }
        curr->display();
        curr = curr->next;
    }
    std::cout << "-------------------------\n";
}

void UndoRedoManager::compareTwoVersions(int version1, int version2) const {
    AudioEditNode* node1 = editHistory.jumpToVersion(version1);
    AudioEditNode* node2 = editHistory.jumpToVersion(version2);
    
    if (!node1) {
        std::cout << "Version " << version1 << " not found.\n";
        return;
    }
    if (!node2) {
        std::cout << "Version " << version2 << " not found.\n";
        return;
    }
    
    std::cout << "\n--- Comparison ---\n";
    std::cout << "Version " << version1 << ":\n";
    node1->display();
    std::cout << "Version " << version2 << ":\n";
    node2->display();
    std::cout << "------------------\n";
}

void UndoRedoManager::searchByAction(const std::string& action) const {
    editHistory.searchByAction(action);
}

void UndoRedoManager::searchByCategory(const std::string& category) const {
    editHistory.searchByCategory(category);
}

bool UndoRedoManager::deleteById(int id) {
    // Pre-check if current is the one being deleted
    bool isCurrent = (current != nullptr && current->editId == id);
    
    bool deleted = editHistory.deleteById(id);
    if (deleted) {
        std::cout << "Edit ID " << id << " deleted successfully.\n";
        
        // If we deleted the node that 'current' was pointing to, 
        // we should safely move 'current' to the previous valid node or end of history.
        if (isCurrent) {
            current = editHistory.tail;
            std::cout << "Current version pointer moved to latest available node.\n";
        }
    } else {
        std::cout << "Edit ID " << id << " not found.\n";
    }
    return deleted;
}

void UndoRedoManager::jumpToVersion(int version) {
    AudioEditNode* target = editHistory.jumpToVersion(version);
    if (target) {
        current = target;
        std::cout << "Jumped to version " << version << ".\n";
    } else {
        std::cout << "Version " << version << " not found.\n";
    }
}

int UndoRedoManager::countVersions() const {
    return editHistory.countNodes();
}

const DoublyLinkedList& UndoRedoManager::getList() const {
    return editHistory;
}

// Loads a node directly into history without triggering branching or ID updates
void UndoRedoManager::loadNode(int id, int ver, const std::string& action, const std::string& cat, const std::string& val, const std::string& ts) {
    AudioEditNode* newNode = new AudioEditNode(id, ver, action, cat, val, ts);
    editHistory.insertAtEnd(newNode);
    current = newNode; // By default, advance current to the last loaded node
}

// Adjusts the ID and Version counters after loading from file
void UndoRedoManager::updateNextIdAndVersion() {
    if (editHistory.tail) {
        nextId = editHistory.tail->editId + 1;
        currentVersion = editHistory.tail->version + 1;
    } else {
        nextId = 1;
        currentVersion = 1;
    }
}
