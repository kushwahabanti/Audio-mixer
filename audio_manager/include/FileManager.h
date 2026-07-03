#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "UndoRedoManager.h"
#include <string>
#include <fstream>
#include <vector>

class FileManager {
private:
    std::string currentProjectFile;
    std::string recentProjectsFile;

public:
    FileManager(const std::string& recentProjectsFilePath = "data/recent_projects.txt");
    ~FileManager();

    // Core file I/O
    bool saveToFile(const UndoRedoManager& manager, const std::string& filename);
    bool loadFromFile(UndoRedoManager& manager, const std::string& filename);

    // Utilities
    void autoSave(const UndoRedoManager& manager);
    void listRecentProjects() const;
    void addRecentProject(const std::string& filename);
    
    // Getters and setters
    void setCurrentProjectFile(const std::string& filename);
    std::string getCurrentProjectFile() const;
};

#endif // FILE_MANAGER_H
