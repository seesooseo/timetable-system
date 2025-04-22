// TimetableManagementMenu.cpp
#include "TimetableManagementMenu.hpp"
#include "InputUtility.hpp"    // ← include the helper
#include <iostream>
#include <string>
#include <optional>
#include <TimeValidation.hpp>
#include "TimeUtility.hpp"   // for minutesToHHMM12(...)
#include "Session.hpp"       // for Session
#include <iostream>

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
        std::cout << "9. Check Room Availability\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← safely read 0–8
        choice = utils::readIntInRange("Enter your choice", 0, 8);

        switch (choice) {
        case 1: {  // Create Timetable
            // 1) pick week
            int week = utils::readIntInRange(
                "Enter week number to create timetable (1–53)", 1, 53);

            // 2) build it
            Timetable tt(week);
            while (true) {
                // 2a) course code
                std::string course;
                std::cout << "Enter course code (or blank to finish): ";
                std::getline(std::cin, course);
                if (course.empty()) break;

                // 2b) session type
                std::string type;
                std::cout << "Enter session type ID: ";
                std::getline(std::cin, type);

                // 2c) day of week
                std::string day;
                std::cout << "Enter day (e.g. Monday): ";
                std::getline(std::cin, day);

                // 2d) start/end times (clash‑checked inside Timetable)
                int start = 0, end = 0;
                getValidTimeInput(start, end);

                // 2e) room, lecturer, group
                std::string room, lecturer, group;
                std::cout << "Enter room ID: ";
                std::getline(std::cin, room);
                std::cout << "Enter lecturer ID: ";
                std::getline(std::cin, lecturer);
                std::cout << "Enter group ID: ";
                std::getline(std::cin, group);

                // 2f) assemble and add (will re‑prompt times if clash)
                Session s{
                  course,
                  type,
                  day,
                  start,
                  end,
                  room,
                  lecturer,
                  group
                };
                tt.addSession(s);
            }

            // 3) try to shove it into your manager (with overwrite prompt)
            while (true) {
                try {
                    timetableManager.createTimetable(week, tt);
                    std::cout << "Timetable for week " << week
                        << " created successfully.\n";
                    break;
                }
                catch (const std::runtime_error& e) {
                    std::cout << e.what()
                        << ". Overwrite existing? (y/n): ";
                    char c; std::cin >> c; std::cin.ignore();
                    if (std::tolower(c) == 'y') {
                        timetableManager.updateTimetable(week, tt);
                        std::cout << "Week " << week << " overwritten.\n";
                        break;
                    }
                    else {
                        // pick a new week and rebuild if you like
                        week = utils::readIntInRange(
                            "Enter a different week number", 1, 53);
                        tt = Timetable(week);
                        std::cout << "Building new timetable for week "
                            << week << ":\n";
                        // you could repeat the above loop or simply abort
                        break;
                    }
                }
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
        case 3: {  // List Timetable for a Week
            int week = utils::readIntInRange("Enter week number to list timetable (1–53)", 1, 53);
            Timetable t = timetableManager.getTimetable(week);
            if (t.getSessions().empty()) {
                std::cout << "No timetable for week " << week << ".\n";
            }
            else {
                t.displayTimetable();
            }
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
        case 9: {
            int week, start, end;
            TimetableManager timetableManager;
            std::string room, day;
            week = utils::readIntInRange("Enter week number to check (1-53)", 1, 53);
            std::cout << "Enter room ID: ";
            std::getline(std::cin, room);
            std::cout << "Enter day (e.g. Monday): ";
            std::getline(std::cin, day);
            getValidTimeInput(start, end);
            bool ok = timetableManager.isRoomAvailable(week, room, day, start, end);
            std::cout << "Room " << room
                << (ok ? " is available.\n" : " is NOT available at that time.\n");
            break;
        }
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
