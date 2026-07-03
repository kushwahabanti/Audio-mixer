#ifndef UNDO_REDO_MANAGER_H
#define UNDO_REDO_MANAGER_H

#include "DoublyLinkedList.h"

class UndoRedoManager {
private:
    DoublyLinkedList editHistory;
    AudioEditNode* current;
    int nextId;
    int currentVersion;

public:
    UndoRedoManager();
    ~UndoRedoManager();

    // Core functionality
    void addEdit(const std::string& action, const std::string& category, const std::string& value);
    bool undo();
    bool redo();

    // Branching scenario
    void branchAfterUndo();

    // Getters and displays
    void getCurrentVersion() const;
    void getHistory() const;
    void compareTwoVersions(int version1, int version2) const;
    
    // Additional features via list delegation
    void searchByAction(const std::string& action) const;
    void searchByCategory(const std::string& category) const;
    bool deleteById(int id);
    void jumpToVersion(int version);
    int countVersions() const;

    // For file manager access
    const DoublyLinkedList& getList() const;
    void loadNode(int id, int ver, const std::string& action, const std::string& cat, const std::string& val, const std::string& ts);
    void updateNextIdAndVersion();
};

#endif // UNDO_REDO_MANAGER_H
