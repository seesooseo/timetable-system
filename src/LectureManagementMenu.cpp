// src/LectureManagementMenu.cpp
#include "LectureManagementMenu.hpp"
#include "LecturerManager.hpp"
#include <limits>

LectureManagementMenu::LectureManagementMenu(ModuleManager& mMgr,
    LecturerManager& lMgr,
    RoomManager& rMgr,
    SessionTypeManager& sMgr,
    std::istream& in,
    std::ostream& out)
    : moduleManager_(mMgr)
    , lecturerManager_(lMgr)
    , roomManager_(rMgr)
    , sessionTypeManager_(sMgr)
    , in_(in)
    , out_(out)
{
    // any additional setup…
}

void LectureManagementMenu::run() {
    int choice = -1;
    do {
        out_ << "\n====== Lecture Management ======\n"
            << "1) Add Lecture\n"
            << "2) Edit Lecture\n"
            << "3) Delete Lecture\n"
            << "4) List All Lectures\n"
            << "0) Back to Main Menu\n"
            << "Choice> ";

        if (!(in_ >> choice)) {
            // handle non‑integer input
            in_.clear();
            in_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            out_ << "Invalid input; please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1:
            // TODO: moduleManager_.addLecture(...);
            out_ << "[Add Lecture not implemented]\n";
            break;
        case 2:
            // TODO: moduleManager_.editLecture(...);
            out_ << "[Edit Lecture not implemented]\n";
            break;
        case 3:
            // TODO: moduleManager_.deleteLecture(...);
            out_ << "[Delete Lecture not implemented]\n";
            break;
        case 4:
            // TODO: moduleManager_.listLectures();
            out_ << "[List Lectures not implemented]\n";
            break;
        case 0:
            // just exit
            break;
        default:
            out_ << "Invalid choice; please select one of the menu options.\n";
        }
    } while (choice != 0);
}
