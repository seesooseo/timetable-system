#include "Persistence.hpp"
#include <json.hpp>
#include <fstream>
#include <Student.hpp>

using json = nlohmann::json;

namespace Persistence {

    // Function to save data to a JSON file
    void saveAll(
        const std::string& filename,
        const ModuleManager& mm,
        const RoomManager& rm,
        const SessionTypeManager& stm,
        const GroupManager& gm,
        const UserManager& um,
        const LecturerManager& lm,
        const TimetableManager& tm
    ) {
        json j;

        try {
            // modules
            j["modules"] = json::array();
            for (auto& m : mm.getModules()) {
                json mj;
                mj["code"] = m.getModuleCode();
                mj["name"] = m.getModuleName();
                mj["desc"] = m.getDescription();
                j["modules"].push_back(std::move(mj));
            }

            // rooms
            j["rooms"] = json::array();
            for (auto& r : rm.getRooms()) {
                json rj;
                rj["id"] = r.getRoomId();
                rj["name"] = r.getRoomName();
                j["rooms"].push_back(std::move(rj));
            }

            // session types
            j["session_types"] = json::array();
            for (auto& s : stm.getSessionTypes()) {
                json sj;
                sj["id"] = s.getTypeId();
                sj["name"] = s.getTypeName();
                j["session_types"].push_back(std::move(sj));
            }

            // groups
            j["groups"] = json::array();
            for (auto& g : gm.getGroups()) {
                json gj;
                gj["id"] = g.getGroupId();
                gj["name"] = g.getGroupName();
                j["groups"].push_back(std::move(gj));
            }

            // admins
            j["admins"] = json::array();
            for (auto* a : um.getAdmins()) {
                json aj;
                aj["id"] = a->getUserID();
                aj["name"] = a->getName();
                j["admins"].push_back(std::move(aj));
            }

            // students
            j["students"] = json::array();
            for (auto* s : um.getStudents()) {
                json sj;
                sj["id"] = s->getUserID();
                sj["name"] = s->getName();
                sj["group"] = s->getGroup();
                j["students"].push_back(std::move(sj));
            }

            // lecturers
            j["lecturers"] = json::array();
            for (auto* l : lm.getLecturers()) {
                json lj;
                lj["id"] = l->getUserID();
                lj["name"] = l->getName();
                lj["modules"] = json::array();
                for (auto& mod : l->getModules())
                    lj["modules"].push_back(mod);
                j["lecturers"].push_back(std::move(lj));
            }

            // timetables
            j["timetables"] = json::array();
            for (const auto& t : tm.getTimetables()) {
                json tj;
                tj["week"] = t.getWeekNumber();
                tj["sessions"] = json::array();
                for (const auto& sess : t.getSessions()) {
                    tj["sessions"].push_back(sess); // uses to_json(Session)
                }
                j["timetables"].push_back(std::move(tj));
            }

            std::ofstream ofs(filename);
            if (ofs.is_open()) {
                ofs << j.dump(2);
            }
            else {
                throw std::runtime_error("Failed to open file for writing: " + filename);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error saving data: " << e.what() << std::endl;
        }
    }

    // Function to load data from a JSON file
    void loadAll(
        const std::string& filename,
        ModuleManager& mm,
        RoomManager& rm,
        SessionTypeManager& stm,
        GroupManager& gm,
        UserManager& um,
        LecturerManager& lm,
        TimetableManager& tm
    ) {
        // Step 1: open
        std::ifstream ifs(filename);
        if (!ifs.is_open()) {
            throw std::runtime_error("Failed to open file: " + filename);
        }

        // Step 2: parse
        nlohmann::json j;
        try {
            ifs >> j;
        }
        catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("JSON parse error: " + std::string(e.what()));
        }

        // Step 3: schema checks
        auto requireArray = [&](const char* key) {
            if (!j.contains(key) || !j[key].is_array()) {
                throw std::runtime_error(std::string("Missing or invalid '") + key + "' section");
            }
            };
        requireArray("modules");
        requireArray("rooms");
        requireArray("session_types");
        requireArray("groups");
        requireArray("admins");
        requireArray("students");
        requireArray("lecturers");
        requireArray("timetables");

        // Modules
        if (j.find("modules") != j.end()) {
            for (auto& mj : j["modules"]) {
                try {
                    Module m;
                    m.setModuleCode(mj.at("code").get<std::string>());
                    m.setModuleName(mj.at("name").get<std::string>());
                    m.setDescription(mj.at("desc").get<std::string>());
                    mm.addModule(std::move(m));
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading module: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'modules' section in JSON" << std::endl;
        }

        // Rooms
        if (j.find("rooms") != j.end()) {
            for (auto& rj : j["rooms"]) {
                try {
                    rm.addRoom({ rj.at("id").get<std::string>(), rj.at("name").get<std::string>() });
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading room: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'rooms' section in JSON" << std::endl;
        }

        // Session Types
        if (j.find("session_types") != j.end()) {
            for (auto& sj : j["session_types"]) {
                try {
                    stm.addSessionType({ sj.at("id").get<std::string>(), sj.at("name").get<std::string>() });
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading session type: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'session_types' section in JSON" << std::endl;
        }

        // Groups
        if (j.find("groups") != j.end()) {
            for (auto& gj : j["groups"]) {
                try {
                    gm.addGroup({ gj.at("id").get<std::string>(), gj.at("name").get<std::string>() });
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading group: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'groups' section in JSON" << std::endl;
        }

        // Admins
        if (j.find("admins") != j.end()) {
            for (auto& aj : j["admins"]) {
                try {
                    auto a = std::make_unique<Admin>(
                        aj.at("id").get<std::string>(),
                        aj.at("name").get<std::string>()
                    );
                    um.addUser(std::move(a));
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading admin: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'admins' section in JSON" << std::endl;
        }

        // Students
        if (j.find("students") != j.end()) {
            for (auto& sj : j["students"]) {
                try {
                    auto s = std::make_unique<Student>(
                        sj.at("id").get<std::string>(),
                        sj.at("name").get<std::string>()
                    );
                    s->setGroup(sj.at("group").get<std::string>());
                    um.addUser(std::move(s));
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading student: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'students' section in JSON" << std::endl;
        }

        // Lecturers
        if (j.find("lecturers") != j.end()) {
            for (auto& lj : j["lecturers"]) {
                try {
                    auto l = std::make_unique<Lecturer>(
                        lj.at("id").get<std::string>(),
                        lj.at("name").get<std::string>()
                    );
                    for (auto& mid : lj.at("modules"))
                        l->addModule(mid.get<std::string>());
                    lm.add(std::move(l));
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading lecturer: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'lecturers' section in JSON" << std::endl;
        }

        // Timetables
        if (j.find("timetables") != j.end()) {
            for (auto& tj : j["timetables"]) {
                try {
                    int wk = tj.at("week").get<int>();
                    Timetable tt(wk);
                    for (auto& sj : tj.at("sessions")) {
                        Session s = sj.get<Session>();
                        tt.forceAddSession(s);
                    }
                    tm.createTimetable(wk, std::move(tt));
                }
                catch (const json::exception& e) {
                    std::cerr << "Error loading timetable: " << e.what() << std::endl;
                }
            }
        }
        else {
            std::cerr << "Missing 'timetables' section in JSON" << std::endl;
        }
    }

} // namespace Persistence
