#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include "AudioEditNode.h"

class DoublyLinkedList {
public:
    AudioEditNode* head;
    AudioEditNode* tail;
    int size;

    DoublyLinkedList();
    ~DoublyLinkedList();

    // Core list operations
    void insertAtEnd(AudioEditNode* node);
    bool deleteById(int id);
    
    // Search operations
    AudioEditNode* searchById(int id) const;
    void searchByAction(const std::string& action) const;
    void searchByCategory(const std::string& category) const; // Added based on requirements
    
    // Traverse and display
    void displayAll() const;
    
    // Utilities
    int countNodes() const;
    AudioEditNode* jumpToVersion(int version) const;
    void clearAll();
    
    // Branching utility: deletes all nodes after the given node
    void deleteAfter(AudioEditNode* node);
};

#endif // DOUBLY_LINKED_LIST_H
