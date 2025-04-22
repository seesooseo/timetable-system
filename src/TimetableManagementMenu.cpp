// TimetableManagementMenu.cpp
#include "TimetableManagementMenu.hpp"
#include "InputUtility.hpp"    // ← include the helper
#include <iostream>
#include <string>
#include <optional>

void timetableManagementMenu(TimetableManager& timetableManager) {
    int choice;
    do {
        std::cout << "\n--- Timetable Management Menu ---\n";
        std::cout << "1. Create Timetable\n";
        std::cout << "2. Update Timetable\n";
        std::cout << "3. List Timetable for a Week\n";
        std::cout << "4. Search for Conflicts\n";
        std::cout << "5. Search Timetable by Keyword\n";
        std::cout << "6. Export Timetable to CSV\n";
        std::cout << "7. Resolve Conflicts\n";
        std::cout << "8. Search (filters)\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← safely read 0–8
        choice = utils::readIntInRange("Enter your choice", 0, 8);

        switch (choice) {
        case 1: {
            int week = utils::readIntInRange("\nEnter week number to create timetable", 1, 53);
            Timetable timetable(week);
            std::string session;
            std::cout << "Enter session details: ";
            std::getline(std::cin, session);
            timetable.addSession(session);
            try {
                timetableManager.createTimetable(week, timetable);
                std::cout << "Timetable created successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 2: {
            int week = utils::readIntInRange("\nEnter week number to update timetable", 1, 53); 
            Timetable timetable(week);
            std::string session;
            std::cout << "Enter new session details: ";
            std::getline(std::cin, session);
            timetable.addSession(session);
            try {
                timetableManager.updateTimetable(week, timetable);
                std::cout << "Timetable updated successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            int week = utils::readIntInRange("\nEnter week number to list timetable", 1, 53);
            timetableManager.listTimetable(week);
            break;
        }
        case 4: {
            std::cout << "\nSearching for timetable conflicts...\n";
            auto conflicts = timetableManager.searchConflicts();
            if (conflicts.empty()) {
                std::cout << "No conflicts detected.\n";
            }
            else {
                for (const auto& c : conflicts) {
                    std::cout << c << "\n";
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
                std::cout << "No sessions found matching the keyword.\n";
            }
            else {
                std::cout << "Sessions matching the keyword:\n";
                for (const auto& r : results) {
                    std::cout << r << "\n";
                }
            }
            break;
        }
        case 6: {
            int week = utils::readIntInRange("\nEnter week number to export timetable", 1, 53);
            std::string filename;
            std::cout << "Enter filename (e.g., timetable.csv): ";
            std::getline(std::cin, filename);
            try {
                timetableManager.exportTimetableToCSV(week, filename);
                std::cout << "Timetable exported successfully to " << filename << "\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error exporting timetable: " << e.what() << "\n";
            }
            break;
        }
        case 7: {
            int week = utils::readIntInRange("\nEnter week number to resolve conflicts", 1, 53);
            try {
                timetableManager.resolveConflictsInTimetable(week);
                std::cout << "Conflict resolution completed.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 8: {
            std::optional<int> wk; std::optional<std::string> mod, room, lec;
            std::string tmp;
            std::cout << "Week (blank = any): ";
            std::getline(std::cin, tmp);
            if (!tmp.empty()) wk = std::stoi(tmp);

            std::cout << "Module code (blank = any): ";
            std::getline(std::cin, tmp);
            if (!tmp.empty()) mod = tmp;

            std::cout << "Room ID (blank = any): ";
            std::getline(std::cin, tmp);
            if (!tmp.empty()) room = tmp;

            std::cout << "Lecturer ID (blank = any): ";
            std::getline(std::cin, tmp);
            if (!tmp.empty()) lec = tmp;

            auto res = timetableManager.search(wk, mod, room, lec);
            if (res.empty()) {
                std::cout << "Nothing found\n";
            }
            else {
                for (const auto& l : res) {
                    std::cout << l << "\n";
                }
            }
            break;
        }
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
