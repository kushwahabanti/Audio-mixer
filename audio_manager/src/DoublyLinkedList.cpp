#include "DoublyLinkedList.h"

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}

DoublyLinkedList::~DoublyLinkedList() {
    clearAll();
}

void DoublyLinkedList::insertAtEnd(AudioEditNode* node) {
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }
    size++;
}

bool DoublyLinkedList::deleteById(int id) {
    AudioEditNode* curr = head;
    while (curr) {
        if (curr->editId == id) {
            if (curr->prev) {
                curr->prev->next = curr->next;
            } else {
                head = curr->next; // Node is head
            }

            if (curr->next) {
                curr->next->prev = curr->prev;
            } else {
                tail = curr->prev; // Node is tail
            }
            
            delete curr;
            size--;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

AudioEditNode* DoublyLinkedList::searchById(int id) const {
    AudioEditNode* curr = head;
    while (curr) {
        if (curr->editId == id) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void DoublyLinkedList::searchByAction(const std::string& action) const {
    AudioEditNode* curr = head;
    bool found = false;
    while (curr) {
        if (curr->actionName == action) {
            curr->display();
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "No edits found with action: " << action << std::endl;
    }
}

void DoublyLinkedList::searchByCategory(const std::string& category) const {
    AudioEditNode* curr = head;
    bool found = false;
    while (curr) {
        if (curr->category == category) {
            curr->display();
            found = true;
        }
        curr = curr->next;
    }
    if (!found) {
        std::cout << "No edits found with category: " << category << std::endl;
    }
}

void DoublyLinkedList::displayAll() const {
    if (!head) {
        std::cout << "The edit history is empty." << std::endl;
        return;
    }
    AudioEditNode* curr = head;
    while (curr) {
        curr->display();
        curr = curr->next;
    }
}

int DoublyLinkedList::countNodes() const {
    return size;
}

AudioEditNode* DoublyLinkedList::jumpToVersion(int version) const {
    AudioEditNode* curr = head;
    while (curr) {
        if (curr->version == version) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void DoublyLinkedList::clearAll() {
    AudioEditNode* curr = head;
    while (curr) {
        AudioEditNode* nextNode = curr->next;
        delete curr;
        curr = nextNode;
    }
    head = tail = nullptr;
    size = 0;
}

void DoublyLinkedList::deleteAfter(AudioEditNode* node) {
    if (!node) {
        clearAll();
        return;
    }
    
    AudioEditNode* curr = node->next;
    while (curr) {
        AudioEditNode* toDelete = curr;
        curr = curr->next;
        delete toDelete;
        size--;
    }
    node->next = nullptr;
    tail = node;
}
