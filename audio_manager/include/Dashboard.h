#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "UndoRedoManager.h"
#include <iostream>

class Dashboard {
public:
    // Core UI rendering
    static void printHeader();
    static void printMenu();
    static void clearScreen();

    // Data rendering
    static void printActivityLog(const UndoRedoManager& manager);
    static void printStats(const UndoRedoManager& manager);
};

#endif // DASHBOARD_H
