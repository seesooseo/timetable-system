// src/LectureManagementMenu.hpp
#pragma once

#include <iostream>
#include "ModuleManager.hpp"
#include "LecturerManager.hpp"
#include "RoomManager.hpp"
#include "SessionTypeManager.hpp"

class LectureManagementMenu {
public:
    /// Construct a menu that drives the given managers,
    /// reading from `in` and writing to `out` (defaulting to std::cin/std::cout).
    LectureManagementMenu(ModuleManager& mMgr,
        LecturerManager& lMgr,
        RoomManager& rMgr,
        SessionTypeManager& sMgr,
        std::istream& in = std::cin,
        std::ostream& out = std::cout);

    /// Show the menu, accept commands, and dispatch them.
    void run();

private:
    ModuleManager& moduleManager_;
    LecturerManager& lecturerManager_;
    RoomManager& roomManager_;
    SessionTypeManager& sessionTypeManager_;
    std::istream& in_;
    std::ostream& out_;
};
