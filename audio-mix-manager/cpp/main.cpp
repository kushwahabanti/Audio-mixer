#include <iostream>
#include <string>
#include <sstream>
#include "DoublyLinkedList.h"

int main() {
    DoublyLinkedList list;
    std::string line;

    // Listen continuously on standard input
    while (std::getline(std::cin, line)) {
        if (line == "EXIT") {
            break;
        }
        
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "ADD") {
            double oldVol, newVol;
            std::string timestamp;
            iss >> oldVol >> newVol;
            std::getline(iss, timestamp);
            
            // Trim leading space from timestamp
            if (!timestamp.empty() && timestamp[0] == ' ') {
                timestamp = timestamp.substr(1);
            }

            list.addEdit(oldVol, newVol, timestamp);
            std::cout << "OK" << std::endl;
        } 
        else if (command == "UNDO") {
            double vol = list.undo();
            std::cout << vol << std::endl;
        } 
        else if (command == "REDO") {
            double vol = list.redo();
            std::cout << vol << std::endl;
        } 
        else if (command == "GET") {
            std::cout << list.getHistoryJSON() << std::endl;
        }
        else if (command == "RESET") {
            list.reset();
            std::cout << "OK" << std::endl;
        }
        else {
            std::cout << "UNKNOWN_COMMAND" << std::endl;
        }
    }

    return 0;
}
