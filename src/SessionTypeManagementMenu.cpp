#include "SessionTypeManagementMenu.hpp"
#include <iostream>
#include <string>

void sessionTypeManagementMenu(SessionTypeManager& sessionTypeManager) {
    int choice = 0;
    do {
        std::cout << "\n--- Session Type Management Menu ---" << std::endl;
        std::cout << "1. Add Session Type" << std::endl;
        std::cout << "2. Update Session Type" << std::endl;
        std::cout << "3. Delete Session Type" << std::endl;
        std::cout << "4. List Session Types" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            std::string id, name;
            std::cout << "\nEnter session type ID: ";
            std::getline(std::cin, id);
            std::cout << "Enter session type (e.g., Lecture, Lab): ";
            std::getline(std::cin, name);
            try {
                sessionTypeManager.addSessionType(SessionType(id, name));
                std::cout << "Session type added successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
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
                std::cout << "Session type updated successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter session type ID to delete: ";
            std::getline(std::cin, id);
            try {
                sessionTypeManager.deleteSessionType(id);
                std::cout << "Session type deleted successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "\nListing all session types:" << std::endl;
            sessionTypeManager.listSessionTypes();
            break;
        }
        case 0:
            std::cout << "Returning to Admin Menu..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
}
