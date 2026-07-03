#include <iostream>
#include <string>
#include <limits>
#include "UndoRedoManager.h"
#include "FileManager.h"
#include "Dashboard.h"

// Helper for generic integer input validation
int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Invalid input. Please enter a valid number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

// Helper for generic string input validation
std::string getStringInput(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty()) {
            return value;
        }
        std::cout << "Input cannot be empty. Please try again.\n";
    }
}

int main() {
    UndoRedoManager manager;
    FileManager fileManager("data/recent_projects.txt");

    bool running = true;
    while (running) {
        Dashboard::printHeader();
        
        std::string currentFile = fileManager.getCurrentProjectFile();
        if (!currentFile.empty()) {
            std::cout << "Current Project: " << currentFile << "\n";
        } else {
            std::cout << "Current Project: [Unsaved / New]\n";
        }

        Dashboard::printMenu();
        int choice = getIntInput("");

        Dashboard::clearScreen();

        switch (choice) {
            case 1: { // Create/Open Project
                std::string filename = getStringInput("Enter project filename (e.g., project.txt): ");
                fileManager.setCurrentProjectFile(filename);
                std::cout << "Project set to: " << filename << "\n";
                break;
            }
            case 2: { // Add Edit
                std::cout << "\n--- Add New Edit ---\n";
                std::string action = getStringInput("Action Name (e.g., Cut, Trim): ");
                std::string category = getStringInput("Category (e.g., Vocals, Drums): ");
                std::string value = getStringInput("Value (e.g., -2dB, 500ms): ");
                manager.addEdit(action, category, value);
                break;
            }
            case 3: // View Current Version
                manager.getCurrentVersion();
                break;
            case 4: // Undo
                manager.undo();
                break;
            case 5: // Redo
                manager.redo();
                break;
            case 6: // View Full History
                Dashboard::printActivityLog(manager);
                break;
            case 7: { // Search by Action
                std::string action = getStringInput("Enter Action to search: ");
                manager.searchByAction(action);
                break;
            }
            case 8: { // Search by Category
                std::string category = getStringInput("Enter Category to search: ");
                manager.searchByCategory(category);
                break;
            }
            case 9: { // Delete Edit by ID
                int id = getIntInput("Enter Edit ID to delete: ");
                manager.deleteById(id);
                break;
            }
            case 10: { // Jump to Version
                int ver = getIntInput("Enter Version number to jump to: ");
                manager.jumpToVersion(ver);
                break;
            }
            case 11: { // Compare Two Versions
                int ver1 = getIntInput("Enter first Version number: ");
                int ver2 = getIntInput("Enter second Version number: ");
                manager.compareTwoVersions(ver1, ver2);
                break;
            }
            case 12: // View Statistics
                Dashboard::printStats(manager);
                break;
            case 13: { // Save to File
                std::string filename = fileManager.getCurrentProjectFile();
                if (filename.empty()) {
                    filename = getStringInput("Enter filename to save as: ");
                    fileManager.setCurrentProjectFile(filename);
                }
                fileManager.saveToFile(manager, filename);
                break;
            }
            case 14: { // Load from File
                std::string filename = getStringInput("Enter filename to load: ");
                fileManager.loadFromFile(manager, filename);
                break;
            }
            case 15: // List Recent Projects
                fileManager.listRecentProjects();
                break;
            case 0: // Exit
                std::cout << "Exiting application...\n";
                fileManager.autoSave(manager);
                running = false;
                break;
            default:
                std::cout << "Invalid choice. Please enter a number between 0 and 15.\n";
                break;
        }

        if (running) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
            Dashboard::clearScreen();
        }
    }

    return 0;
}
