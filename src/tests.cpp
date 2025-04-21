// tests.cpp
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <vector>
#include <stdexcept>

// project headers
#include "Module.hpp"
#include "ModuleManager.hpp"
#include "User.hpp"
#include "Admin.hpp"
#include "Student.hpp"
#include "UserManager.hpp"
#include "Group.hpp"
#include "GroupManager.hpp"
#include "SessionType.hpp"
#include "SessionTypeManager.hpp"
#include "Room.hpp"
#include "RoomManager.hpp"
#include "Timetable.hpp"
#include "TimetableManager.hpp"
#include "AuthManager.hpp"
#include "Session.hpp"
#include "TimeUtility.hpp"
#include "TimeValidation.hpp"

void testModule() {
    Module m("C101", "Name", "Desc");
    assert(m.getModuleCode() == "C101");
    assert(m.getModuleName() == "Name");
    assert(m.getDescription() == "Desc");

    m.setModuleCode("X");
    m.setModuleName("Y");
    m.setDescription("Z");
    assert(m.getModuleCode() == "X");
    assert(m.getModuleName() == "Y");
    assert(m.getDescription() == "Z");
}

void testModuleManager() {
    ModuleManager mgr;
    Module m("M1", "N1", "D1");

    // add/search
    mgr.addModule(m);
    auto out = mgr.searchModule("M1");
    assert(out.getModuleCode() == "M1");

    // duplicate
    bool threw = false;
    try { mgr.addModule(m); }
    catch (...) { threw = true; }
    assert(threw);

    // update
    Module m2("M1", "NN", "DD");
    mgr.updateModule("M1", m2);
    assert(mgr.searchModule("M1").getModuleName() == "NN");

    // delete
    mgr.deleteModule("M1");
    threw = false;
    try { mgr.searchModule("M1"); }
    catch (...) { threw = true; }
    assert(threw);
}

void testUserPolymorphismAndManager() {
    UserManager um;
    // Admin
    auto a = std::make_unique<Admin>("A1", "AA");
    um.addUser(std::move(a));
    User* found = um.findUserById("A1");
    assert(found && found->getName() == "AA");

    // Student
    auto s = std::make_unique<Student>("S1", "BB");
    um.addUser(std::move(s));
    found = um.findUserById("S1");
    assert(found && found->getName() == "BB");

    // remove non-existent
    bool threw = false;
    try { um.removeUser("???"); }
    catch (...) { threw = true; }
    assert(threw);

    // remove existing
    um.removeUser("A1");
    assert(um.findUserById("A1") == nullptr);
}

void testGroupManager() {
    GroupManager gm;
    Group g1("G1", "One"), g2("G2", "Two");
    gm.addGroup(g1);
    gm.addGroup(g2);

    bool threw = false;
    try { gm.addGroup(g1); }
    catch (...) { threw = true; }
    assert(threw);

    Group g1u("G1", "OneU");
    gm.updateGroup("G1", g1u);

    gm.deleteGroup("G2");
    threw = false;
    try { gm.deleteGroup("G2"); }
    catch (...) { threw = true; }
    assert(threw);
}

void testSessionTypeManager() {
    SessionTypeManager stm;
    SessionType st1("T1", "Lec"), st2("T2", "Lab");
    stm.addSessionType(st1);
    stm.addSessionType(st2);

    bool threw = false;
    try { stm.addSessionType(st1); }
    catch (...) { threw = true; }
    assert(threw);

    SessionType st1u("T1", "Tut");
    stm.updateSessionType("T1", st1u);

    stm.deleteSessionType("T2");
    threw = false;
    try { stm.deleteSessionType("T2"); }
    catch (...) { threw = true; }
    assert(threw);
}

void testRoomManager() {
    RoomManager rm;
    Room r1("R1", "One"), r2("R2", "Two");
    rm.addRoom(r1);
    rm.addRoom(r2);

    bool threw = false;
    try { rm.addRoom(r1); }
    catch (...) { threw = true; }
    assert(threw);

    Room r1u("R1", "Main");
    rm.updateRoom("R1", r1u);

    rm.deleteRoom("R2");
    threw = false;
    try { rm.deleteRoom("R2"); }
    catch (...) { threw = true; }
    assert(threw);
}

void testSessionParsingAndTimeUtility() {
    Session s = parseSession("C1 Lec Mon 600 750 R1");
    assert(s.courseName == "C1");
    assert(s.sessionType == "Lec");
    assert(s.day == "Mon");
    assert(s.startTime == 600 && s.endTime == 750);
    assert(s.roomId == "R1");

    // Time formatting
    assert(minutesToHHMM12(0) == "12:00 AM");
    assert(minutesToHHMM12(600) == "10:00 AM");
    assert(minutesToHHMM12(750) == "12:30 PM");
    assert(minutesToHHMM12(13 * 60 + 5) == "01:05 PM");
}

void testTimetableAndManager() {
    Timetable t(1);
    t.addSession("M1 Lec Mon 600 660 R1");
    t.addSession("M2 Lec Mon 650 710 R1");

    TimetableManager tm;
    tm.createTimetable(1, t);

    bool threw = false;
    try { tm.createTimetable(1, t); }
    catch (...) { threw = true; }
    assert(threw);

    // get
    auto got = tm.getTimetable(1);
    assert(got.getWeekNumber() == 1);
    assert(got.getSessions().size() == 2);

    // conflicts
    auto conflicts = tm.searchConflicts();
    assert(conflicts.size() == 1);

    // keyword
    auto kw = tm.searchTimetableByKeyword("M1");
    assert(!kw.empty());

    // export CSV
    const char* fn = "test_export.csv";
    tm.exportTimetableToCSV(1, fn);
    std::ifstream in(fn);
    assert(in.good());
    in.close();
    std::remove(fn);

    // resolve conflicts
    tm.resolveConflictsInTimetable(1);
    auto conflicts2 = tm.searchConflicts();
    assert(conflicts2.empty());
}

void testAuthManager() {
    UserManager um;
    um.addUser(std::make_unique<Admin>("AX", "A"));
    AuthManager am(um);

    User* u1 = am.loginUser("AX");
    assert(u1 && u1->getUserID() == "AX");

    User* u2 = am.loginUser("ZZ");
    assert(u2 == nullptr);
}

void testValidateTime() {
    bool thrown1 = false;
    bool thrown2 = false;

    // Case 1: start > end should throw
    try {
        validateTimes(700, 699);
    }
    catch (const std::runtime_error&) {
        thrown1 = true;
    }

    // Case 2: negative start should throw
    try {
        validateTimes(-10, 30);
    }
    catch (const std::runtime_error&) {
        thrown2 = true;
    }

    if (thrown1 && thrown2) {
        std::cout << "testValidateTime: PASS\n";
    }
    else {
        std::cout << "testValidateTime: FAIL ("
            << "thrown1=" << thrown1 << ", "
            << "thrown2=" << thrown2 << ")\n";
    }
}

int main() {
    testModule();
    testModuleManager();
    testUserPolymorphismAndManager();
    testGroupManager();
    testSessionTypeManager();
    testRoomManager();
    testSessionParsingAndTimeUtility();
    testTimetableAndManager();
    testAuthManager();
    testValidateTime();
    std::cout << "All tests passed.\n";
    return 0;
}
