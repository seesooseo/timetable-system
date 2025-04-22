// tests.cpp
#include <sys/stat.h>
#include <sys/types.h>
#ifdef _WIN32
#include <direct.h>
#endif

#include <cassert>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <vector>
#include <stdexcept>
#include <sstream>

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
#include <cstdlib>
#include <Lecturer.hpp>

using json = nlohmann::json;

void ensureExportsDirectory() {
#ifdef _WIN32
    _mkdir("exports");
#else
    mkdir("exports", 0777);
#endif
}

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

    std::cout << "testModule: PASS\n";
}

void testModuleManager() {
    ModuleManager mgr;
    Module m("M1", "N1", "D1");
    mgr.addModule(m);
    auto out = mgr.searchModule("M1");
    assert(out.getModuleCode() == "M1");

    bool threw = false;
    try { mgr.addModule(m); }
    catch (...) { threw = true; }
    assert(threw);

    Module m2("M1", "NN", "DD");
    mgr.updateModule("M1", m2);
    assert(mgr.searchModule("M1").getModuleName() == "NN");

    mgr.deleteModule("M1");
    threw = false;
    try { mgr.searchModule("M1"); }
    catch (...) { threw = true; }
    assert(threw);

    std::cout << "testModuleManager: PASS\n";
}

void testUserPolymorphismAndManager() {
    UserManager um;
    auto a = std::make_unique<Admin>("A1", "AA");
    um.addUser(std::move(a));
    User* found = um.findUserById("A1");
    assert(found && found->getName() == "AA");

    auto s = std::make_unique<Student>("S1", "BB");
    um.addUser(std::move(s));
    found = um.findUserById("S1");
    assert(found && found->getName() == "BB");

    bool threw = false;
    try { um.removeUser("???"); }
    catch (...) { threw = true; }
    assert(threw);

    um.removeUser("A1");
    assert(um.findUserById("A1") == nullptr);

    std::cout << "testUserPolymorphismAndManager: PASS\n";
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

    std::cout << "testGroupManager: PASS\n";
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

    std::cout << "testSessionTypeManager: PASS\n";
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

    std::cout << "testRoomManager: PASS\n";
}

void testSessionParsingAndTimeUtility() {
    Session s = parseSession("C1 Lec Mon 600 750 R1");
    assert(s.courseName == "C1");
    assert(s.sessionType == "Lec");
    assert(s.day == "Mon");
    assert(s.startTime == 600 && s.endTime == 750);
    assert(s.roomId == "R1");

    assert(minutesToHHMM12(0) == "12:00 AM");
    assert(minutesToHHMM12(600) == "10:00 AM");
    assert(minutesToHHMM12(750) == "12:30 PM");
    assert(minutesToHHMM12(13 * 60 + 5) == "01:05 PM");

    std::cout << "testSessionParsingAndTimeUtility: PASS\n";
}

// Uncomment if you want to include this test run
void testTimetableAndManager() {
    Timetable t(1);

    Session s1 = parseSession("M1 Lec Mon 600 660 R1 L1 G1");
    Session s2 = parseSession("M2 Lec Mon 650 710 R1 L1 G1");

    t.forceAddSession(s1);
    t.forceAddSession(s2);

    TimetableManager tm;
    tm.createTimetable(1, t);

    bool threw = false;
    try { tm.createTimetable(1, t); }
    catch (...) { threw = true; }
    assert(threw);

    auto got = tm.getTimetable(1);
    assert(got.getWeekNumber() == 1);
    assert(got.getSessions().size() == 2);

    auto conflicts = tm.searchConflicts();
    assert(conflicts.size() == 1);

    auto kw = tm.searchTimetableByKeyword("M1");
    assert(!kw.empty());

    ensureExportsDirectory();
    const char* fn = "test_export.csv";
    tm.exportTimetableToCSV(1, fn);
    std::ifstream in("./exports/test_export.csv");
    assert(in.good());
    in.close();
    std::remove("./exports/test_export.csv");

    tm.resolveConflictsInTimetable(1);
    auto conflicts2 = tm.searchConflicts();
    assert(conflicts2.empty());

    std::cout << "testTimetableAndManager: PASS\n";
}


void testAuthManager() {
    UserManager um;
    um.addUser(std::make_unique<Admin>("AX", "A"));
    AuthManager am(um);

    const User* u1 = am.loginUser("AX");
    assert(u1 && u1->getUserID() == "AX");

    const User* u2 = am.loginUser("ZZ");
    assert(u2 == nullptr);

    std::cout << "testAuthManager: PASS\n";
}

void testValidateTime() {
    bool thrown1 = false, thrown2 = false, thrown3 = false;

    try { getValidSessionTimes(700, 699); }  // invalid: start > end
    catch (...) { thrown1 = true; }

    try { getValidSessionTimes(-1, 20); }    // invalid: negative start
    catch (...) { thrown2 = true; }

    try { getValidSessionTimes(500, 600); }  // valid
    catch (...) { thrown3 = true; }

    if (thrown1 && thrown2 && !thrown3) {
        std::cout << "testValidateTime: PASS\n";
    }
    else {
        std::cout << "testValidateTime: FAIL\n";
    }
}


void testLecturerInline() {
    Lecturer L("L1", "Dr Who");
    assert(L.getModules().empty());

    L.displayUserInfo();

    L.addModule("COMP101");
    auto mods = L.getModules();
    assert(mods.size() == 1 && mods[0] == "COMP101");

    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    L.displayUserInfo();
    std::cout.rdbuf(old);
    std::string s = out.str();
    assert(s.find("COMP101") != std::string::npos);
    assert(s.find("Lecturer") != std::string::npos);

    std::cout << "testLecturerInline: PASS\n";
}

void testLecturerManager() {
    LecturerManager mgr;

    mgr.add(std::make_unique<Lecturer>("LX", "Prof X"));
    Lecturer* p = mgr.find("LX");
    assert(p && p->getUserID() == "LX");

    bool threw = false;
    try { mgr.add(std::make_unique<Lecturer>("LX", "Prof X")); }
    catch (...) { threw = true; }
    assert(threw);

    mgr.remove("LX");
    assert(mgr.find("LX") == nullptr);

    threw = false;
    try { mgr.remove("LX"); }
    catch (...) { threw = true; }
    assert(threw);

    mgr.add(std::make_unique<Lecturer>("LA", "Alpha"));
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    mgr.list();
    std::cout.rdbuf(old);
    std::string s = out.str();
    assert(s.find("Lecturer") != std::string::npos);
    assert(s.find("Alpha") != std::string::npos);

    std::cout << "testLecturerManager: PASS\n";
}

void testLectureManagementMenu() {
    ModuleManager mMgr;
    LecturerManager lMgr;
    RoomManager rMgr;
    SessionTypeManager sMgr;

    std::istringstream in1("0\n");
    std::ostringstream out1;
    LectureManagementMenu menu1(mMgr, lMgr, rMgr, sMgr, in1, out1);
    menu1.run();

    std::istringstream in2("foo\n0\n");
    std::ostringstream out2;
    LectureManagementMenu menu2(mMgr, lMgr, rMgr, sMgr, in2, out2);
    menu2.run();
    assert(out2.str().find("Invalid input") != std::string::npos);

    for (int opt = 1; opt <= 4; ++opt) {
        std::istringstream in(std::to_string(opt) + "\n0\n");
        std::ostringstream out;
        LectureManagementMenu menu(mMgr, lMgr, rMgr, sMgr, in, out);
        menu.run();
        std::string s = out.str();
        switch (opt) {
        case 1: assert(s.find("[Add Lecture") != std::string::npos); break;
        case 2: assert(s.find("[Edit Lecture") != std::string::npos); break;
        case 3: assert(s.find("[Delete Lecture") != std::string::npos); break;
        case 4: assert(s.find("[List Lectures") != std::string::npos); break;
        }
    }

    std::cout << "testLectureManagementMenu: PASS\n";
}

void testModuleHeader() {
    Module m0;
    assert(m0.getModuleCode().empty());
    assert(m0.getModuleName().empty());
    assert(m0.getDescription().empty());

    Module m("C101", "Intro", "Basics");
    assert(m.getModuleCode() == "C101");

    Module m2 = m;
    assert(m2.getDescription() == "Basics");

    m.setModuleCode("X1");
    m.setModuleName("NameX");
    m.setDescription("DescX");

    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    m.displayModule();
    std::cout.rdbuf(old);
    std::string printed = out.str();
    assert(printed.find("X1") != std::string::npos);

    std::cout << "testModuleHeader: PASS\n";
}

void testRoomHeader() {
    Room r0;
    assert(r0.getRoomId().empty());

    Room r("R01", "Lab A");
    assert(r.getRoomName() == "Lab A");

    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    r.displayRoom();
    std::cout.rdbuf(old);
    std::string printed = out.str();
    assert(printed.find("Lab A") != std::string::npos);

    std::cout << "testRoomHeader: PASS\n";
}

void testGetters() {
    {
        ModuleManager m;
        m.addModule({ "M1","Name","Desc" });
        assert(m.getModules().size() == 1);
    }
    {
        RoomManager r;
        r.addRoom({ "R1","Room One" });
        assert(r.getRooms().size() == 1);
    }
    {
        SessionTypeManager s;
        s.addSessionType({ "T1","Lab" });
        assert(s.getSessionTypes().size() == 1);
    }
    {
        GroupManager g;
        g.addGroup({ "G1","Group" });
        assert(g.getGroups().size() == 1);
    }
    {
        LecturerManager lm;
        lm.add(std::make_unique<Lecturer>("L1", "Dr"));
        assert(lm.getLecturers().size() == 1);
    }
    {
        TimetableManager tm;
        Timetable t(1);
        tm.createTimetable(1, t);
        assert(tm.getTimetables().size() == 1);
    }

    std::cout << "testGetters: PASS\n";
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
    Session s = parseSession("CS101 Lecture Mon 600 660 R1 L1 G1");
    t.forceAddSession(s);
    tm.createTimetable(1, t);

    Persistence::saveAll("test_data.json", mm, rm, stm, gm, um, lm, tm);

    TimetableManager tm2;
    Persistence::loadAll("test_data.json", mm, rm, stm, gm, um, lm, tm2);

    std::cout << "testPersistenceRoundTrip: PASS\n";
}

void testStudent() {
    Student s("S42", "Jane");
    assert(s.getUserID() == "S42");
    assert(s.getName() == "Jane");

    s.displayUserInfo();

    s.setUserID("S99");
    s.setName("Janet");
    assert(s.getUserID() == "S99");
    assert(s.getName() == "Janet");

    std::cout << "testStudent: PASS\n";
}

void testSession() {
    Session s = { "C1", "Lec", "Wed", 800, 900, "R3", "L3", "G3" };
    assert(s.courseName == "C1");
    assert(s.sessionType == "Lec");
    assert(s.day == "Wed");
    assert(s.startTime == 800);
    assert(s.endTime == 900);
    assert(s.roomId == "R3");
    assert(s.lecturerId == "L3");
    assert(s.groupId == "G3");

    std::cout << "testSession: PASS\n";
}

void testTimeValidationEdgeCases() {
    try {
        getValidSessionTimes(600, 700); // valid
    }
    catch (...) {
        assert(false);
    }

    bool threw = false;
    try {
        getValidSessionTimes(750, 600); // invalid: end < start
    }
    catch (...) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        getValidSessionTimes(-100, 200); // invalid: negative
    }
    catch (...) {
        threw = true;
    }
    assert(threw);

    std::cout << "testTimeValidationEdgeCases: PASS\n";
}

void testAdmin() {
    Admin a("admin01", "Alice");
    assert(a.getUserID() == "admin01");
    assert(a.getName() == "Alice");
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    a.displayUserInfo();
    std::cout.rdbuf(old);
    assert(out.str().find("admin01") != std::string::npos);
    std::cout << "testAdmin: PASS\n";
}

void testGroupHeader() {
    Group g;
    assert(g.getGroupId().empty());
    assert(g.getGroupName().empty());

    Group g2("G01", "TestGroup");
    assert(g2.getGroupId() == "G01");
    assert(g2.getGroupName() == "TestGroup");

    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    g2.displayGroup();
    std::cout.rdbuf(old);
    std::string s = out.str();
    assert(s.find("TestGroup") != std::string::npos);
    std::cout << "testGroupHeader: PASS\n";
}

void testModuleManagerHeader() {
    ModuleManager mm;
    assert(mm.getModules().empty());
    std::cout << "testModuleManagerHeader: PASS\n";
}

void testPersistenceStubs() {
    ModuleManager mm;
    RoomManager rm;
    SessionTypeManager stm;
    GroupManager gm;
    UserManager um;
    LecturerManager lm;
    TimetableManager tm;

    Persistence::saveAll("test_stub.json", mm, rm, stm, gm, um, lm, tm);
    Persistence::loadAll("test_stub.json", mm, rm, stm, gm, um, lm, tm);
    std::cout << "testPersistenceStubs: PASS\n";
}

void testSessionTypeHeader() {
    SessionType st;
    assert(st.getTypeId().empty());
    assert(st.getTypeName().empty());
    std::cout << "testSessionTypeHeader: PASS\n";
}

void testStudentHeader() {
    Student s;
    assert(s.getUserID().empty());
    assert(s.getName().empty());
    std::ostringstream out;
    auto* old = std::cout.rdbuf(out.rdbuf());
    s.displayUserInfo();
    std::cout.rdbuf(old);
    assert(out.str().find("Student") != std::string::npos);
    std::cout << "testStudentHeader: PASS\n";
}

void testTimetableHeader() {
    Timetable t;
    assert(t.getWeekNumber() == 0);
    assert(t.getSessions().empty());
    std::cout << "testTimetableHeader: PASS\n";
}

void testTimeUtilityEdgeCases() {
    assert(minutesToHHMM12(1439) == "11:59 PM");
    assert(minutesToHHMM12(720) == "12:00 PM");
    std::cout << "testTimeUtilityEdgeCases: PASS\n";
}

void testUserHeader() {
    Admin a("x", "y");
    assert(a.getUserID() == "x");
    assert(a.getName() == "y");
    std::cout << "testUserHeader: PASS\n";
}

void testUserManagerHeader() {
    UserManager um;
    assert(um.getAdmins().empty());
    assert(um.getStudents().empty());
    assert(um.findUserById("X") == nullptr);
    std::cout << "testUserManagerHeader: PASS\n";
}

int main() {
    testModule();
    testModuleManager();
    testUserPolymorphismAndManager();
    testGroupManager();
    testSessionTypeManager();
    testRoomManager();
    testSessionParsingAndTimeUtility();
    testTimetableAndManager();  // optional
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
    testStudent();
    testSession();
    testTimeValidationEdgeCases();
    testAdmin();
    testGroupHeader();
    testModuleManagerHeader();
    testPersistenceStubs();
    testStudentHeader();
    testTimetableHeader();
    testTimeUtilityEdgeCases();
    testUserHeader();
    testUserManagerHeader();
    std::cout << "All tests passed.\n";
    return 0;
}
