#include "AudioEditNode.h"
#include <chrono>
#include <iomanip>

// Constructor for a new edit
AudioEditNode::AudioEditNode(int id, int ver, const std::string& action, const std::string& cat, const std::string& val)
    : editId(id), version(ver), actionName(action), category(cat), value(val), prev(nullptr), next(nullptr) {
    timestamp = currentTimestamp();
}

// Constructor for loading from a file (timestamp provided)
AudioEditNode::AudioEditNode(int id, int ver, const std::string& action, const std::string& cat, const std::string& val, const std::string& ts)
    : editId(id), version(ver), actionName(action), category(cat), value(val), timestamp(ts), prev(nullptr), next(nullptr) {
}

// Displays the node information
void AudioEditNode::display() const {
    std::cout << "[" << timestamp << "] "
              << "V" << version << " | "
              << "ID: " << editId << " | "
              << "Action: " << actionName << " | "
              << "Category: " << category << " | "
              << "Value: " << value << std::endl;
}

// Generates a current timestamp string
std::string AudioEditNode::currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
