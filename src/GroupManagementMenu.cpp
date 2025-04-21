#include "GroupManagementMenu.hpp"
#include <iostream>
#include <string>

void groupManagementMenu(GroupManager& groupManager) {
    int choice = 0;
    do {
        std::cout << "\n--- Group Management Menu ---" << std::endl;
        std::cout << "1. Add Group" << std::endl;
        std::cout << "2. Update Group" << std::endl;
        std::cout << "3. Delete Group" << std::endl;
        std::cout << "4. List Groups" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            std::string id, name;
            std::cout << "\nEnter group ID: ";
            std::getline(std::cin, id);
            std::cout << "Enter group name: ";
            std::getline(std::cin, name);
            try {
                groupManager.addGroup(Group(id, name));
                std::cout << "Group added successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 2: {
            std::string id, name;
            std::cout << "\nEnter group ID to update: ";
            std::getline(std::cin, id);
            std::cout << "Enter new group name: ";
            std::getline(std::cin, name);
            try {
                groupManager.updateGroup(id, Group(id, name));
                std::cout << "Group updated successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter group ID to delete: ";
            std::getline(std::cin, id);
            try {
                groupManager.deleteGroup(id);
                std::cout << "Group deleted successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "\nListing all groups:" << std::endl;
            groupManager.listGroups();
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
