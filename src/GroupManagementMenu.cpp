// GroupManagementMenu.cpp
#include "GroupManagementMenu.hpp"
#include "InputUtility.hpp"    // ← include the helper
#include <iostream>
#include <string>

void groupManagementMenu(GroupManager& groupManager) {
    int choice;
    do {
        std::cout << "\n--- Group Management Menu ---\n";
        std::cout << "1. Add Group\n";
        std::cout << "2. Update Group\n";
        std::cout << "3. Delete Group\n";
        std::cout << "4. List Groups\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← use our helper to safely read 0–4
        choice = utils::readIntInRange("Enter your choice", 0, 4);

        switch (choice) {
        case 1: {
            std::string groupID =utils::promptValidID('G', "Group");
            std::string name;
            std::cout << "Enter group name: ";
            std::getline(std::cin, name);
            try {
                groupManager.addGroup(Group(groupID, name));
                std::cout << "Group added successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
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
                std::cout << "Group updated successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter group ID to delete: ";
            std::getline(std::cin, id);
            try {
                groupManager.deleteGroup(id);
                std::cout << "Group deleted successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 4:
            std::cout << "\nListing all groups:\n";
            groupManager.listGroups();
            break;
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
