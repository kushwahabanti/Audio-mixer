#ifndef AUDIO_EDIT_NODE_H
#define AUDIO_EDIT_NODE_H

#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

struct AudioEditNode {
    int editId;
    int version;
    std::string actionName;
    std::string category;
    std::string value;
    std::string timestamp;
    
    AudioEditNode* prev;
    AudioEditNode* next;

    // Constructor
    AudioEditNode(int id, int ver, const std::string& action, const std::string& cat, const std::string& val);
    
    // Constructor for loading from file (includes timestamp)
    AudioEditNode(int id, int ver, const std::string& action, const std::string& cat, const std::string& val, const std::string& ts);

    // Displays the node information
    void display() const;

    // Generates a current timestamp string
    static std::string currentTimestamp();
};

#endif // AUDIO_EDIT_NODE_H
