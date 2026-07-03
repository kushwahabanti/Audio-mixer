#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>

FileManager::FileManager(const std::string& recentProjectsFilePath) 
    : currentProjectFile(""), recentProjectsFile(recentProjectsFilePath) {}

FileManager::~FileManager() {}

void FileManager::setCurrentProjectFile(const std::string& filename) {
    currentProjectFile = filename;
}

std::string FileManager::getCurrentProjectFile() const {
    return currentProjectFile;
}

bool FileManager::saveToFile(const UndoRedoManager& manager, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for saving.\n";
        return false;
    }

    const DoublyLinkedList& list = manager.getList();
    AudioEditNode* curr = list.head;

    while (curr) {
        outFile << curr->editId << "|"
                << curr->version << "|"
                << curr->actionName << "|"
                << curr->category << "|"
                << curr->value << "|"
                << curr->timestamp << "\n";
        curr = curr->next;
    }

    outFile.close();
    std::cout << "Project saved successfully to " << filename << ".\n";
    addRecentProject(filename);
    return true;
}

bool FileManager::loadFromFile(UndoRedoManager& manager, const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file " << filename << " for loading.\n";
        return false;
    }

    std::string line;
    int nodesLoaded = 0;

    // Clear current state before loading
    // We assume load is called on a fresh manager, or we clear it first (omitted here for safety, but ideal)

    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        while (std::getline(ss, token, '|')) {
            tokens.push_back(token);
        }

        if (tokens.size() == 6) {
            int id = std::stoi(tokens[0]);
            int ver = std::stoi(tokens[1]);
            manager.loadNode(id, ver, tokens[2], tokens[3], tokens[4], tokens[5]);
            nodesLoaded++;
        } else {
            std::cerr << "Warning: Skipping malformed line in file.\n";
        }
    }

    inFile.close();
    manager.updateNextIdAndVersion(); // Ensure IDs and Versions are correct for next edits
    
    std::cout << "Loaded " << nodesLoaded << " edits from " << filename << ".\n";
    addRecentProject(filename);
    currentProjectFile = filename;
    return true;
}

void FileManager::autoSave(const UndoRedoManager& manager) {
    if (!currentProjectFile.empty()) {
        std::cout << "Auto-saving project...\n";
        saveToFile(manager, currentProjectFile);
    } else {
        std::cout << "No project file associated. Auto-save skipped.\n";
    }
}

void FileManager::addRecentProject(const std::string& filename) {
    // Basic implementation to append recent project
    std::ofstream outFile(recentProjectsFile, std::ios::app);
    if (outFile.is_open()) {
        outFile << filename << "\n";
        outFile.close();
    }
}

void FileManager::listRecentProjects() const {
    std::ifstream inFile(recentProjectsFile);
    if (!inFile.is_open()) {
        std::cout << "No recent projects found.\n";
        return;
    }

    std::cout << "\n--- Recent Projects ---\n";
    std::string line;
    std::vector<std::string> projects;
    while (std::getline(inFile, line)) {
        if (!line.empty() && std::find(projects.begin(), projects.end(), line) == projects.end()) {
            projects.push_back(line);
        }
    }

    if (projects.empty()) {
        std::cout << "No recent projects found.\n";
    } else {
        // Display last 5 unique projects (or less)
        int start = projects.size() > 5 ? projects.size() - 5 : 0;
        for (size_t i = start; i < projects.size(); ++i) {
            std::cout << i - start + 1 << ". " << projects[i] << "\n";
        }
    }
    std::cout << "-----------------------\n";
    inFile.close();
}
