// SessionTypeManagementMenu.cpp
#include "SessionTypeManagementMenu.hpp"
#include "InputUtility.hpp"    // ← include the helper
#include <iostream>
#include <string>

void sessionTypeManagementMenu(SessionTypeManager& sessionTypeManager) {
    int choice;
    do {
        std::cout << "\n--- Session Type Management Menu ---\n";
        std::cout << "1. Add Session Type\n";
        std::cout << "2. Update Session Type\n";
        std::cout << "3. Delete Session Type\n";
        std::cout << "4. List Session Types\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← safely read 0–4
        choice = utils::readIntInRange("Enter your choice", 0, 4);

        switch (choice) {
        case 1: {
            std::string id, name;
            std::cout << "\nEnter session type ID: ";
            std::getline(std::cin, id);
            std::cout << "Enter session type (e.g., Lecture, Lab): ";
            std::getline(std::cin, name);
            try {
                sessionTypeManager.addSessionType(SessionType(id, name));
                std::cout << "Session type added successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 2: {
            std::string id, name;
            std::cout << "\nEnter session type ID to update: ";
            std::getline(std::cin, id);
            std::cout << "Enter new session type: ";
            std::getline(std::cin, name);
            try {
                sessionTypeManager.updateSessionType(id, SessionType(id, name));
                std::cout << "Session type updated successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter session type ID to delete: ";
            std::getline(std::cin, id);
            try {
                sessionTypeManager.deleteSessionType(id);
                std::cout << "Session type deleted successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 4:
            std::cout << "\nListing all session types:\n";
            sessionTypeManager.listSessionTypes();
            break;
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
