#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <iostream>
#include <string>
#include <ctime>
#include <vector>

// The Node class representing a single volume edit
class Node {
public:
    double oldVolume;
    double newVolume;
    std::string timestamp;
    Node* prev;
    Node* next;

    Node(double oVol, double nVol, const std::string& ts) {
        oldVolume = oVol;
        newVolume = nVol;
        timestamp = ts;
        prev = nullptr;
        next = nullptr;
    }
};

// The core DSA Logic class
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    Node* current;

public:
    DoublyLinkedList() {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }

    ~DoublyLinkedList() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
    }

    // New edit removes future branch
    void clearRedoHistory() {
        if (current != nullptr) {
            Node* temp = current->next;
            while (temp != nullptr) {
                Node* toDelete = temp;
                temp = temp->next;
                delete toDelete;
            }
            current->next = nullptr;
            tail = current;
        } else {
            // If current is null, we are at the initial state, delete everything
            Node* temp = head;
            while (temp != nullptr) {
                Node* nextNode = temp->next;
                delete temp;
                temp = nextNode;
            }
            head = nullptr;
            tail = nullptr;
        }
    }

    void addEdit(double oldVol, double newVol, const std::string& timestamp) {
        clearRedoHistory();
        
        Node* newNode = new Node(oldVol, newVol, timestamp);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
            current = newNode;
        } else {
            current->next = newNode;
            newNode->prev = current;
            tail = newNode;
            current = newNode;
        }
    }

    // Returns the target volume to revert to, or -1 if nothing to undo
    double undo() {
        if (current == nullptr) return -1.0; 
        
        double targetVolume = current->oldVolume;
        current = current->prev;
        
        return targetVolume;
    }

    // Returns the target volume to advance to, or -1 if nothing to redo
    double redo() {
        Node* nextNode = (current != nullptr) ? current->next : head;
        
        if (nextNode == nullptr) return -1.0; 
        
        current = nextNode;
        return current->newVolume;
    }

    // Helper for IPC visualization / History panel
    std::string getHistoryJSON() {
        std::string json = "[";
        Node* temp = head;
        while (temp != nullptr) {
            bool isCurrent = (temp == current);
            
            json += "{";
            json += "\"oldVolume\":" + std::to_string(temp->oldVolume) + ",";
            json += "\"newVolume\":" + std::to_string(temp->newVolume) + ",";
            json += "\"timestamp\":\"" + temp->timestamp + "\",";
            json += "\"isCurrent\":" + std::string(isCurrent ? "true" : "false");
            json += "}";
            
            if (temp->next != nullptr) {
                json += ",";
            }
            temp = temp->next;
        }
        json += "]";
        
        // Append extra info about current state
        std::string currentIsNull = (current == nullptr) ? "true" : "false";
        return "{\"history\":" + json + ", \"currentIsNull\":" + currentIsNull + "}";
    }
    
    // Clear everything (Reset Project)
    void reset() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        current = nullptr;
    }
};

#endif
