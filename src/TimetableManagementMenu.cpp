#include "TimetableManagementMenu.hpp"
#include <iostream>
#include <string>

void timetableManagementMenu(TimetableManager& timetableManager) {
    int choice = 0;
    do {
        std::cout << "\n--- Timetable Management Menu ---" << std::endl;
        std::cout << "1. Create Timetable" << std::endl;
        std::cout << "2. Update Timetable" << std::endl;
        std::cout << "3. List Timetable for a Week" << std::endl;
        std::cout << "4. Search for Conflicts" << std::endl;
        std::cout << "5. Search Timetable by Keyword" << std::endl;
        std::cout << "6. Export Timetable to CSV" << std::endl;
        std::cout << "7. Resolve Conflicts" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;
        std::cout << "8. Search (filters)" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            int week;
            std::cout << "\nEnter week number to create timetable: ";
            std::cin >> week;
            if (week < 1 || week > 53) {
                std::cout << "Week must be between 1 and 53.\n";
                break;
            }
            std::cin.ignore();
            Timetable timetable(week);
            // For simplicity, we prompt for one session; you could extend this as needed.
            std::string session;
            std::cout << "Enter session details: ";
            std::getline(std::cin, session);
            timetable.addSession(session);
            try {
                timetableManager.createTimetable(week, timetable);
                std::cout << "Timetable created successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 2: {
            int week;
            std::cout << "\nEnter week number to update timetable: ";
            std::cin >> week;
            if (week < 1 || week > 53) {
                std::cout << "Week must be between 1 and 53.\n";
                break;
            }
            std::cin.ignore();
            Timetable timetable(week);
            std::string session;
            std::cout << "Enter new session details: ";
            std::getline(std::cin, session);
            timetable.addSession(session);
            try {
                timetableManager.updateTimetable(week, timetable);
                std::cout << "Timetable updated successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            int week;
            std::cout << "\nEnter week number to list timetable: ";
            std::cin >> week;
            if (week < 1 || week > 53) {
                std::cout << "Week must be between 1 and 53.\n";
                break;
            }
            std::cin.ignore();
            timetableManager.listTimetable(week);
            break;
        }
        case 4: {
            std::cout << "\nSearching for timetable conflicts..." << std::endl;
            auto conflicts = timetableManager.searchConflicts();
            if (conflicts.empty()) {
                std::cout << "No conflicts detected." << std::endl;
            }
            else {
                for (const auto& conflict : conflicts) {
                    std::cout << conflict << std::endl;
                }
            }
            break;
        }
        case 5: {
            std::cout << "\nEnter keyword to search for: ";
            std::string keyword;
            std::getline(std::cin, keyword);
            auto results = timetableManager.searchTimetableByKeyword(keyword);
            if (results.empty()) {
                std::cout << "No sessions found matching the keyword." << std::endl;
            }
            else {
                std::cout << "Sessions matching the keyword:" << std::endl;
                for (const auto& r : results) {
                    std::cout << r << std::endl;
                }
            }
            break;
        }
        case 6: {
            int week;
            std::cout << "\nEnter week number to export timetable: ";
            std::cin >> week;
            if (week < 1 || week > 53) {
                std::cout << "Week must be between 1 and 53.\n";
                break;
            }
            std::cin.ignore();
            std::string filename;
            std::cout << "Enter filename (e.g., timetable.csv): ";
            std::getline(std::cin, filename);
            try {
                timetableManager.exportTimetableToCSV(week, filename);
                std::cout << "Timetable exported successfully to " << filename << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error exporting timetable: " << e.what() << std::endl;
            }
            break;
        }
              // In TimetableManagementMenu.cpp, add another case:
        case 7: {
            int week;
            std::cout << "\nEnter week number to resolve conflicts: ";
            std::cin >> week;
            if (week < 1 || week > 53) {
                std::cout << "Week must be between 1 and 53.\n";
                break;
            }
            std::cin.ignore();
            try {
                timetableManager.resolveConflictsInTimetable(week);
                std::cout << "Conflict resolution completed." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 8: {
            std::optional<int> wk; std::optional<std::string> mod, room, lec;
            std::string tmp;
            std::cout << "Week (blank = any): "; std::getline(std::cin, tmp);
            if (!tmp.empty()) wk = std::stoi(tmp);

            std::cout << "Module code (blank = any): "; std::getline(std::cin, tmp);
            if (!tmp.empty()) mod = tmp;

            std::cout << "Room ID (blank = any): "; std::getline(std::cin, tmp);
            if (!tmp.empty()) room = tmp;

            std::cout << "Lecturer ID (blank = any): "; std::getline(std::cin, tmp);
            if (!tmp.empty()) lec = tmp;

            auto res = timetableManager.search(wk, mod, room, lec);
            if (res.empty()) std::cout << "Nothing found\n";
            else for (auto& l : res) std::cout << l << "\n";
            break;
        }
        case 0:
            std::cout << "Returning to Admin Menu..." << std::endl;
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);
}
