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
#include "LectureManagementMenu.hpp"
#include "LecturerManager.hpp"
#include <InputUtility.hpp>
#include <Persistence.hpp>
#include <SampleData.hpp>


// Forward declarations for menus and login.
void adminExtendedMenu(
    ModuleManager& moduleManager,
    UserManager& userManager,
    GroupManager& groupManager,
    SessionTypeManager& sessionTypeManager,
    RoomManager& roomManager,
    TimetableManager& timetableManager,
    LecturerManager& lectureManager
);
void studentMenu(ModuleManager& moduleManager, UserManager& userManager, TimetableManager& timetableManager);


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

    // containers for persistence
    std::vector<Module>         modules;
    std::vector<Room>           rooms;
    std::vector<SessionType>    sessionTypes;
    std::vector<Group>          groups;
    std::vector<std::unique_ptr<Admin>>    admins;
    std::vector<std::unique_ptr<Student>>  students;
    std::vector<std::unique_ptr<Lecturer>> lecturers;
    std::vector<Timetable>      timetables;


    // now initialize managers using loaded data
    ModuleManager        moduleManager;
    RoomManager          roomManager;
    SessionTypeManager   sessionTypeManager;
    GroupManager         groupManager;
    UserManager          userManager;
    LecturerManager      lecturerManager;
    TimetableManager     timetableManager;

    

    for (auto& m : modules)        moduleManager.addModule(m);
    for (auto& r : rooms)          roomManager.addRoom(r);
    for (auto& s : sessionTypes)   sessionTypeManager.addSessionType(s);
    for (auto& g : groups)         groupManager.addGroup(g);
    for (auto& a : admins)         userManager.addUser(std::move(a));
    for (auto& s : students)       userManager.addUser(std::move(s));
    for (auto& l : lecturers)      lecturerManager.add(std::move(l));
    for (auto& tt : timetables)     timetableManager.createTimetable(tt.getWeekNumber(), tt);



    AuthManager authManager(userManager);

    // Main menu loop for login.
    while (true) {
        generateSampleJSON("sample.json");
        Persistence::loadAll(
            "sample.json",
            moduleManager,
            roomManager,
            sessionTypeManager,
            groupManager,
            userManager,
            lecturerManager,
            timetableManager
        );

        if (userManager.getAdmins().empty()) {
            // file had no admin entries → seed at least one
            populateDefaultData(
                userManager,
                moduleManager,
                groupManager,
                sessionTypeManager,
                roomManager,
                timetableManager,
                lecturerManager
            );
            Persistence::saveAll(
                "sample.json",
                moduleManager,
                roomManager,
                sessionTypeManager,
                groupManager,
                userManager,
                lecturerManager,
                timetableManager
            );
        }
        std::cout << "\n=== NTU Timetabling System ===\n";
        std::cout << "1. Login\n";
        std::cout << "0. Exit\n";

        // ← replaces raw std::cin >> mainChoice
        int mainChoice = utils::readIntInRange("Enter your choice", 0, 1);

        if (mainChoice == 0) {
            std::cout << "Exiting system. Goodbye!\n";
            break;
        }
        else if (mainChoice == 1) {
            std::cout << "\n=== Login ===\nEnter User ID: ";
            std::string userId;
            std::getline(std::cin, userId);
            // (Optionally, you could add a simple non‑empty check here)
            User* currentUser = authManager.loginUser(userId);
            if (currentUser) {
                if (dynamic_cast<Admin*>(currentUser)) {
                    adminExtendedMenu(moduleManager, userManager, groupManager,
                        sessionTypeManager, roomManager, timetableManager,
                        lecturerManager);
                }
                else if (dynamic_cast<Student*>(currentUser)) {
                    studentMenu(moduleManager, userManager, timetableManager);
                }
            }
        }
        else {
            // This case will never trigger, since readIntInRange enforces 0–1
        }
    }
    Persistence::saveAll("data.json",
        moduleManager,  // add getters for all lists if needed
        roomManager,
        sessionTypeManager,
        groupManager,
        userManager,
        lecturerManager,
        timetableManager);
    return 0;
}

// A simple login function that searches for a user by user ID.
User* loginUser( UserManager& userManager) {
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
void studentMenu(ModuleManager& moduleManager, UserManager& userManager, TimetableManager& timetableManager) {
    int choice = 0;
    do {
        std::cout << "\n=== Student Menu ===" << std::endl;
        std::cout << "1. View Modules" << std::endl;
        std::cout << "2. View Weekly Schedule" << std::endl;
        std::cout << "3. Export Schedule as CSV" << std::endl;
        std::cout << "0. Return to Main Menu" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1:
            moduleManager.listModules();
            break;
        case 2: {
            std::string groupId;
            std::cout << "Enter your group ID to view your timetable: ";
            std::getline(std::cin, groupId);

            int week;
            std::cout << "Enter week number (1–54): ";
            std::cin >> week;
            std::cin.ignore();

            try {
                Timetable timetable = timetableManager.getTimetable(week);
                timetable.displayForGroup(groupId);  // displayForGroup already handles display logic for the student's group
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }

        case 3: {
            std::string groupId;
            std::cout << "Enter your group ID to export timetable: ";
            std::getline(std::cin, groupId);

            int week;
            std::cout << "Enter week number (1–54): ";
            std::cin >> week;
            std::cin.ignore();

            std::string filename = "Group_" + groupId + "_Week" + std::to_string(week) + ".csv";

            try {
                timetableManager.exportTimetableToCSV(week, filename);
                std::cout << "Timetable exported successfully to " << filename << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
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
            userManagementMenu(userManager, groupManager);
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
        case 7: {
              LectureManagementMenu lectureMenu(
                  moduleManager,
                  lecturerManager,
                  roomManager,
                  sessionTypeManager,
                  std::cin,
                  std::cout
              );

              // now actually launch it (use your real member function name here)
              lectureMenu.run();           // or menu.showMenu(); or menu.display(); etc.
              break;
            };
        case 0:
            std::cout << "Returning to Main Menu..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
            break;
        }
    } while (choice != 0);

}

