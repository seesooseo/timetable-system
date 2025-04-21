#include "UserManagementMenu.hpp"
#include "Admin.hpp"
#include "Student.hpp"

void userManagementMenu(UserManager& userManager) {
    int choice = 0;
    do {
        std::cout << "\n--- User Management Menu ---" << std::endl;
        std::cout << "1. Add User" << std::endl;
        std::cout << "2. Delete User" << std::endl;
        std::cout << "3. Search User by ID" << std::endl;
        std::cout << "4. List Users" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;
        std::cout << "5 Assign student to group" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

        switch (choice) {
        case 1: {
            std::string id, name, role;
            std::cout << "\nEnter new user ID: ";
            std::getline(std::cin, id);
            std::cout << "Enter new user name: ";
            std::getline(std::cin, name);
            std::cout << "Enter user role (admin/student): ";
            std::getline(std::cin, role);

            try {
                if (role == "admin") {
                    userManager.addUser(std::make_unique<Admin>(id, name));
                }
                else if (role == "student") {
                    userManager.addUser(std::make_unique<Student>(id, name));
                }
                else {
                    std::cout << "Invalid role. Use 'admin' or 'student'." << std::endl;
                }
                std::cout << "User added successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 2: {
            std::string id;
            std::cout << "\nEnter user ID to delete: ";
            std::getline(std::cin, id);
            try {
                userManager.removeUser(id);
                std::cout << "User deleted successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter user ID to search: ";
            std::getline(std::cin, id);
            User* user = userManager.findUserById(id);
            if (user) {
                std::cout << "User found:" << std::endl;
                user->displayUserInfo();
            }
            else {
                std::cout << "User with ID " << id << " not found." << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "\nListing all users:" << std::endl;
            userManager.listUsers();
            break;
        }
        case 5: {
            std::string sid, gid;
            std::cout << "Student ID: "; std::getline(std::cin, sid);
            std::cout << "Group ID: ";   std::getline(std::cin, gid);
            try {
                userManager.assignStudentToGroup(sid, gid);
            }
            catch (std::exception& e) { std::cout << e.what() << "\n"; }
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
