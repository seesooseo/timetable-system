#include <iostream>
#include <memory>

// Include all manager classes and user/entity classes.
#include "AuthManager.hpp"          // Handles login; in separate auth_manager.hpp/.cpp files.
#include "ModuleManager.hpp"        // For module CRUD.
#include "UserManager.hpp"          // For managing Admin and Student users.
#include "GroupManager.hpp"         // For managing student groups.
#include "SessionTypeManager.hpp"   // For managing session types.
#include "RoomManager.hpp"          // For managing rooms.
#include "TimetableManager.hpp"     // For managing timetables.
#include "Admin.hpp"                // Derived from User.
#include "Student.hpp"              // Derived from User.
#include "Group.hpp"                // Group entity.
#include "SessionType.hpp"          // SessionType entity.
#include "Room.hpp"                 // Room entity.
#include "Timetable.hpp"            // Timetable entity.
#include "ModuleManagementMenu.hpp"
#include "UserManagementMenu.hpp"
#include "GroupManagementMenu.hpp"
#include "SessionTypeManagementMenu.hpp"
#include "RoomManagementMenu.hpp"
#include "TimetableManagementMenu.hpp"
#include <LecturerManagementMenu.hpp>
#include <LecturerManager.hpp>


// Forward declarations for menus and login.
void adminExtendedMenu(
    ModuleManager& moduleManager,
    UserManager& userManager,
    GroupManager& groupManager,
    SessionTypeManager& sessionTypeManager,
    RoomManager& roomManager,
    TimetableManager& timetableManager,
    LecturerManager& lecturerManager
);
void studentMenu(ModuleManager& moduleManager, UserManager& userManager);


// This function populates default data into all manager instances.
void populateDefaultData(
    UserManager& userManager,
    ModuleManager& moduleManager,
    GroupManager& groupManager,
    SessionTypeManager& sessionTypeManager,
    RoomManager& roomManager,
    TimetableManager& timetableManager,
    LecturerManager& lecturerManager
) {
    try {
        // Default users
        userManager.addUser(std::make_unique<Admin>("A001", "Admin One"));
        userManager.addUser(std::make_unique<Student>("S001", "Student One"));

        // Default modules
        moduleManager.addModule(Module("COMP101", "Intro to Programming", "Basic C++ programming concepts."));
        moduleManager.addModule(Module("COMP102", "Data Structures", "Introduction to data structures such as arrays, lists, stacks, and queues."));

        // Default groups
        groupManager.addGroup(Group("G001", "Group A"));
        groupManager.addGroup(Group("G002", "Group B"));

        // Default session types (Lecture, Lab, etc.)
        sessionTypeManager.addSessionType(SessionType("ST01", "Lecture"));
        sessionTypeManager.addSessionType(SessionType("ST02", "Lab"));

        // Default rooms
        roomManager.addRoom(Room("R001", "John Clare Lecture Theatre 006"));
        roomManager.addRoom(Room("R002", "MAE 202"));

        // Default timetable for week 1
        Timetable week1(1);
        week1.addSession("COMP101 Lecture Monday 600 660 R001");
        week1.addSession("COMP102 Lecture Monday 650 710 R001");
        timetableManager.createTimetable(1, week1);

        lecturerManager.add(std::make_unique<Lecturer>("L001", "Dr Smith"));
        // Link lecturer to module COMP101
        Module* mod = moduleManager.findModule("COMP101");
        if (mod) {
            mod->addLecturer("L001");
        }

        std::cout << "Default data populated successfully." << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error populating default data: " << e.what() << std::endl;
    }
}

int main() {
    // Instantiate manager objects once.
    ModuleManager moduleManager;
    UserManager userManager;
    GroupManager groupManager;
    SessionTypeManager sessionTypeManager;
    RoomManager roomManager;
    TimetableManager timetableManager;
    LecturerManager lecturerManager;

    // Populate default data into the system.
    populateDefaultData(userManager, moduleManager, groupManager,
        sessionTypeManager, roomManager, timetableManager, lecturerManager);

    AuthManager authManager(userManager);

    // Main menu loop for login.
    while (true) {
        std::cout << "\n=== NTU Timetabling System ===" << std::endl;
        std::cout << "1. Login" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";

        int mainChoice;
        std::cin >> mainChoice;
        std::cin.ignore();

        if (mainChoice == 0) {
            std::cout << "Exiting system. Goodbye!" << std::endl;
            break;
        }
        else if (mainChoice == 1) {
            std::cout << "\n=== Login ===\nEnter User ID: ";
            // Use AuthManager's login function; for simplicity here, using a simple loginUser.
            std::string userId;
            std::cin >> userId;
            std::cin.ignore();
            User* currentUser = authManager.loginUser(userId);
            if (currentUser) {
                // Direct user to the appropriate menu based on their role.
                if (dynamic_cast<Admin*>(currentUser) != nullptr) {
                    adminExtendedMenu(moduleManager, userManager, groupManager,
                        sessionTypeManager, roomManager, timetableManager, lecturerManager);
                }
                else if (dynamic_cast<Student*>(currentUser) != nullptr) {
                    studentMenu(moduleManager, userManager);
                }
            }
        }
        else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }

    return 0;
}

// A simple login function that searches for a user by user ID.
User* loginUser(UserManager& userManager) {
    std::string userId;
    std::cin >> userId;
    std::cin.ignore();
    User* user = userManager.findUserById(userId);
    if (!user) {
        std::cout << "Login failed. User with ID " << userId << " does not exist." << std::endl;
    }
    else {
        std::cout << "Login successful! Welcome, " << user->getName() << "." << std::endl;
    }
    return user;
}

// A basic student menu for demonstration.
void studentMenu(ModuleManager& moduleManager, UserManager& userManager) {
    int choice = 0;
    do {
        std::cout << "\n=== Student Menu ===" << std::endl;
        std::cout << "1. View Modules" << std::endl;
        // Future options for timetable search and CSV export can be added.
        std::cout << "0. Return to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            moduleManager.listModules();
            break;
        case 0:
            std::cout << "Returning to Main Menu..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
        }
    } while (choice != 0);
}

// Extended admin menu that includes options for all Phase D features.
void adminExtendedMenu(
    ModuleManager& moduleManager,
    UserManager& userManager,
    GroupManager& groupManager,
    SessionTypeManager& sessionTypeManager,
    RoomManager& roomManager,
    TimetableManager& timetableManager,
    LecturerManager& lecturerManager
) {
    int choice = 0;
    do {
        std::cout << "\n=== Admin Menu (Extended) ===" << std::endl;
        std::cout << "1. Module Management" << std::endl;
        std::cout << "2. User Management" << std::endl;
        std::cout << "3. Group Management" << std::endl;
        std::cout << "4. Session Type Management" << std::endl;
        std::cout << "5. Room Management" << std::endl;
        std::cout << "6. Timetable Management" << std::endl;
        std::cout << "0. Return to Main Menu" << std::endl;
        std::cout << "7. Lecturer Management" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            std::cout << "\n--- Module Management ---" << std::endl;
            moduleManagementMenu(moduleManager);
            break;
        case 2:
            std::cout << "\n--- User Management ---" << std::endl;
            userManagementMenu(userManager);
            break;
        case 3:
            std::cout << "\n--- Group Management ---" << std::endl;
            groupManagementMenu(groupManager);
            break;
        case 4:
            std::cout << "\n--- Session Type Management ---" << std::endl;
            sessionTypeManagementMenu(sessionTypeManager);
            break;
        case 5:
            std::cout << "\n--- Room Management ---" << std::endl;
            roomManagementMenu(roomManager);
            break;
        case 6: {
            std::cout << "\n--- Timetable Management ---" << std::endl;
            timetableManagementMenu(timetableManager);
            break;
        }
        case 7:
            lecturerManagementMenu(lecturerManager);
            break;
        case 0:
            std::cout << "Returning to Main Menu..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
            break;
        }
    } while (choice != 0);
}
