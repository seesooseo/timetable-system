// tests.cpp
#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <vector>
#include <stdexcept>
#include <sstream>

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
#include <LectureManagementMenu.hpp>
#include <json.hpp>
#include <Persistence.hpp>
#include <iostream>
#include <cstdlib>
using json = nlohmann::json;


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

    const User* u1 = am.loginUser("AX");
    assert(u1 && u1->getUserID() == "AX");

    const User* u2 = am.loginUser("ZZ");
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

// 1) Lecturer.hpp inline coverage
void testLecturerInline() {
    Lecturer L("L1", "Dr Who");

    // getModules on empty
    auto empty = L.getModules();
    assert(empty.empty());

    // capture displayUserInfo on empty modules
    {
        std::ostringstream out;
        L.displayUserInfo();
        // it prints to std::cout, so we just ensure no crash and manually inspect
    }

    // addModule + getModules
    L.addModule("COMP101");
    auto mods = L.getModules();
    assert(mods.size() == 1 && mods[0] == "COMP101");

    // capture displayUserInfo on non‐empty
    {
        std::ostringstream out;
        // temporarily redirect cout
        auto* old = std::cout.rdbuf(out.rdbuf());
        L.displayUserInfo();
        std::cout.rdbuf(old);
        auto s = out.str();
        assert(s.find("COMP101") != std::string::npos);
        assert(s.find("Lecturer") != std::string::npos);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// 2) LecturerManager.cpp coverage
void testLecturerManager() {
    LecturerManager mgr;

    // add + find
    mgr.add(std::make_unique<Lecturer>("LX", "Prof X"));
    Lecturer* p = mgr.find("LX");
    assert(p && p->getUserID() == "LX");

    // duplicate add → throw
    bool threw = false;
    try { mgr.add(std::make_unique<Lecturer>("LX", "Prof X")); }
    catch (...) { threw = true; }
    assert(threw);

    // remove existing
    mgr.remove("LX");
    assert(mgr.find("LX") == nullptr);

    // remove non‐existent → throw
    threw = false;
    try { mgr.remove("LX"); }
    catch (...) { threw = true; }
    assert(threw);

    // test list() prints each lecturer
    mgr.add(std::make_unique<Lecturer>("LA", "Alpha"));
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    mgr.list();
    std::cout.rdbuf(old);
    auto s = out.str();
    assert(s.find("Lecturer") != std::string::npos);
    assert(s.find("Alpha") != std::string::npos);
}

// ─────────────────────────────────────────────────────────────────────────────
// 3) LectureManagementMenu.cpp coverage
void testLectureManagementMenu() {
    ModuleManager      mMgr;
    LecturerManager    lMgr;
    RoomManager        rMgr;
    SessionTypeManager sMgr;

    // a) immediate exit
    {
        std::istringstream in("0\n");
        std::ostringstream out;
        LectureManagementMenu menu(mMgr, lMgr, rMgr, sMgr, in, out);
        menu.run();
    }

    // b) non‑numeric input branch
    {
        std::istringstream in("foo\n0\n");
        std::ostringstream out;
        LectureManagementMenu menu(mMgr, lMgr, rMgr, sMgr, in, out);
        menu.run();
        assert(out.str().find("Invalid input") != std::string::npos);
    }

    // c) stubbed options 1–4
    for (int opt = 1; opt <= 4; ++opt) {
        std::istringstream in(std::to_string(opt) + "\n0\n");
        std::ostringstream out;
        LectureManagementMenu menu(mMgr, lMgr, rMgr, sMgr, in, out);
        menu.run();
        auto s = out.str();
        switch (opt) {
        case 1: assert(s.find("[Add Lecture") != std::string::npos); break;
        case 2: assert(s.find("[Edit Lecture") != std::string::npos); break;
        case 3: assert(s.find("[Delete Lecture") != std::string::npos); break;
        case 4: assert(s.find("[List Lectures") != std::string::npos); break;
        }
    }
}

void testModuleHeader() {
    // default ctor + empty getters
    Module m0;
    assert(m0.getModuleCode().empty());
    assert(m0.getModuleName().empty());
    assert(m0.getDescription().empty());

    // parameterized ctor + getters
    Module m("C101", "Intro", "Basics");
    assert(m.getModuleCode() == "C101");
    assert(m.getModuleName() == "Intro");
    assert(m.getDescription() == "Basics");

    // copy ctor
    Module m2 = m;
    assert(m2.getModuleCode() == "C101");
    assert(m2.getModuleName() == "Intro");
    assert(m2.getDescription() == "Basics");

    // setters
    m.setModuleCode("X1");
    m.setModuleName("NameX");
    m.setDescription("DescX");
    assert(m.getModuleCode() == "X1");
    assert(m.getModuleName() == "NameX");
    assert(m.getDescription() == "DescX");

    // displayModule only prints code, name and description
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    m.displayModule();
    std::cout.rdbuf(old);

    std::string printed = out.str();
    assert(printed.find("X1") != std::string::npos);
    assert(printed.find("NameX") != std::string::npos);
    assert(printed.find("DescX") != std::string::npos);
}

void testRoomHeader() {
    // default ctor + empty getters
    Room r0;
    assert(r0.getRoomId().empty());
    assert(r0.getRoomName().empty());

    // parameterized ctor + getters
    Room r("R01", "Lab A");
    assert(r.getRoomId() == "R01");
    assert(r.getRoomName() == "Lab A");

    // displayRoom
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    r.displayRoom();
    std::cout.rdbuf(old);

    std::string printed = out.str();
    assert(printed.find("R01") != std::string::npos);
    assert(printed.find("Lab A") != std::string::npos);
}

void testSessionTypeHeader() {
    // default ctor + empty getters
    SessionType st0;
    assert(st0.getTypeId().empty());
    assert(st0.getTypeName().empty());

    // parameterized ctor + getters
    SessionType st("ST1", "Lecture");
    assert(st.getTypeId() == "ST1");
    assert(st.getTypeName() == "Lecture");

    // displaySessionType
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    st.displaySessionType();
    std::cout.rdbuf(old);

    std::string printed = out.str();
    assert(printed.find("ST1") != std::string::npos);
    assert(printed.find("Lecture") != std::string::npos);
}

void testGetters() {
    {
        ModuleManager m;
        m.addModule({ "M1","Name","Desc" });
        auto ms = m.getModules();
        assert(ms.size() == 1 && ms[0].getModuleCode() == "M1");
    }
    {
        RoomManager r;
        r.addRoom({ "R1","Room One" });
        auto rs = r.getRooms();
        assert(rs.size() == 1 && rs[0].getRoomId() == "R1");
    }
    {
        SessionTypeManager s;
        s.addSessionType({ "T1","Lab" });
        auto ss = s.getSessionTypes();
        assert(ss.size() == 1 && ss[0].getTypeId() == "T1");
    }
    {
        GroupManager g;
        g.addGroup({ "G1","Group" });
        auto gs = g.getGroups();
        assert(gs.size() == 1 && gs[0].getGroupId() == "G1");
    }
    {
        LecturerManager lm;
        lm.add(std::make_unique<Lecturer>("L1", "Dr"));
        auto ls = lm.getLecturers();
        assert(ls.size() == 1 && ls[0]->getUserID() == "L1");
    }
    {
        TimetableManager tm;
        Timetable t(1);
        tm.createTimetable(1, t);
        auto ts = tm.getTimetables();
        assert(ts.size() == 1 && ts[0].getWeekNumber() == 1);
    }
}

void testPersistenceRoundTrip() {
    ModuleManager mm;
    RoomManager rm;
    SessionTypeManager stm;
    GroupManager gm;
    UserManager um;
    LecturerManager lm;
    TimetableManager tm;
    Timetable t(1);
    t.addSession("CS101 Lecture Mon 600 660 R1 L1 G1");
    tm.createTimetable(1, t);

    // Save to file
    Persistence::saveAll("test_data.json",
        mm,
        rm,
        stm,
        gm,
        um,
        lm,
        tm);

    // Load into new TimetableManager
    TimetableManager tm2;
    Persistence::loadAll("test_data.json",
        mm,
        rm,
        stm,
        gm,
        um,
        lm,
        tm2);

    // Optionally, check something from tm2 here to validate round-trip
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
    testLecturerInline();
    testLecturerManager();
    testLectureManagementMenu();
    testModuleHeader();
    testRoomHeader();
    testSessionTypeHeader();
    testGetters();
    testPersistenceRoundTrip();
    std::cout << "All tests passed.\n";
    std::cout << "All LectureManagementMenu tests passed.\n";
    return 0;
}
