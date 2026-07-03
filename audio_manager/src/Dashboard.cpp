#include "Dashboard.h"
#include <cstdlib>

void Dashboard::printHeader() {
    std::cout << "=================================================\n";
    std::cout << "           AUDIO MANAGER (UNDO/REDO)             \n";
    std::cout << "=================================================\n";
}

void Dashboard::printMenu() {
    std::cout << "\n[1] Create/Open Project\n"
              << "[2] Add Edit\n"
              << "[3] View Current Version\n"
              << "[4] Undo\n"
              << "[5] Redo\n"
              << "[6] View Full History\n"
              << "[7] Search by Action\n"
              << "[8] Search by Category\n"
              << "[9] Delete Edit by ID\n"
              << "[10] Jump to Version\n"
              << "[11] Compare Two Versions\n"
              << "[12] View Statistics\n"
              << "[13] Save to File\n"
              << "[14] Load from File\n"
              << "[15] List Recent Projects\n"
              << "[0] Exit (Auto-saves if project is open)\n"
              << "\nChoose an option: ";
}

void Dashboard::clearScreen() {
    // Clear screen for both Windows and Unix
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void Dashboard::printActivityLog(const UndoRedoManager& manager) {
    std::cout << "\n=== ACTIVITY LOG ===\n";
    manager.getHistory(); // The manager handles the actual node traversal
}

void Dashboard::printStats(const UndoRedoManager& manager) {
    std::cout << "\n=== STATISTICS ===\n";
    std::cout << "Total Edits in History: " << manager.countVersions() << "\n";
    
    // We could add more stats here (like count by category) if needed.
    // For now, displaying the total count is sufficient.
    std::cout << "==================\n";
}
