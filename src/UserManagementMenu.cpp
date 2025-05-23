﻿// UserManagementMenu.cpp
#include "UserManagementMenu.hpp"
#include "InputUtility.hpp"    // ← include the helper
#include "Admin.hpp"
#include "Student.hpp"
#include <GroupManager.hpp>

void userManagementMenu(UserManager& userManager, GroupManager& groupManager) {
    int choice;
    do {
        std::cout << "\n--- User Management Menu ---\n";
        std::cout << "1. Add User\n";
        std::cout << "2. Delete User\n";
        std::cout << "3. Search User by ID\n";
        std::cout << "4. List Users\n";
        std::cout << "5. Assign Student to Group\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← replaces raw std::cin >> choice
        choice = utils::readIntInRange("Enter your choice", 0, 5);

        switch (choice) {
        case 1: {
            std::string userID = utils::promptValidID('U', "User");
            std::string name, role;
            std::cout << "Enter new user name: ";
            std::getline(std::cin, name);
            std::cout << "Enter user role (admin/student): ";
            std::getline(std::cin, role);
            try {
                if (role == "admin") {
                    userManager.addUser(std::make_unique<Admin>(userID, name));
                }
                else if (role == "student") {
                    userManager.addUser(std::make_unique<Student>(userID, name));
                }
                else {
                    std::cout << "Invalid role. Use 'admin' or 'student'.\n";
                }
                std::cout << "User added successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 2: {
            std::string id;
            std::cout << "\nEnter user ID to delete: ";
            std::getline(std::cin, id);
            try {
                userManager.removeUser(id);
                std::cout << "User deleted successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter user ID to search: ";
            std::getline(std::cin, id);
            const User* user = userManager.findUserById(id);
            if (user) {
                std::cout << "User found:\n";
                user->displayUserInfo();
            }
            else {
                std::cout << "User with ID " << id << " not found.\n";
            }
            break;
        }
        case 4:
            std::cout << "\nListing all users:\n";
            userManager.listUsers();
            break;
        case 5: {
            std::string sid, gid;
            std::cout << "Student ID: ";
            std::getline(std::cin, sid);
            std::cout << "Group ID: ";
            std::getline(std::cin, gid);
            try {
                userManager.assignStudentToGroup(sid, gid, groupManager);
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
