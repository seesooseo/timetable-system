#include "Persistence.hpp"
#include <json.hpp>
#include <fstream>
#include <Student.hpp>

using json = nlohmann::json;

namespace Persistence {

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
        ofs << j.dump(2);
    }

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
        std::ifstream ifs(filename);
        json j; ifs >> j;

        for (auto& mj : j["modules"]) {
            Module m;
            m.setModuleCode(mj.at("code").get<std::string>());
            m.setModuleName(mj.at("name").get<std::string>());
            m.setDescription(mj.at("desc").get<std::string>());
            mm.addModule(std::move(m));
        }

        for (auto& rj : j["rooms"]) {
            rm.addRoom({ rj.at("id").get<std::string>(), rj.at("name").get<std::string>() });
        }

        for (auto& sj : j["session_types"]) {
            stm.addSessionType({ sj.at("id").get<std::string>(), sj.at("name").get<std::string>() });
        }

        for (auto& gj : j["groups"]) {
            gm.addGroup({ gj.at("id").get<std::string>(), gj.at("name").get<std::string>() });
        }

        for (auto& aj : j["admins"]) {
            auto a = std::make_unique<Admin>(
                aj.at("id").get<std::string>(),
                aj.at("name").get<std::string>()
            );
            um.addUser(std::move(a));
        }

        for (auto& sj : j["students"]) {
            auto s = std::make_unique<Student>(
                sj.at("id").get<std::string>(),
                sj.at("name").get<std::string>()
            );
            s->setGroup(sj.at("group").get<std::string>());
            um.addUser(std::move(s));
        }

        for (auto& lj : j["lecturers"]) {
            auto l = std::make_unique<Lecturer>(
                lj.at("id").get<std::string>(),
                lj.at("name").get<std::string>()
            );
            for (auto& mid : lj.at("modules"))
                l->addModule(mid.get<std::string>());
            lm.add(std::move(l));
        }

        for (auto& tj : j["timetables"]) {
            int wk = tj.at("week").get<int>();
            Timetable tt(wk);
            for (auto& sj : tj.at("sessions")) {
                Session s = sj.get<Session>();
                tt.addSession(s);
            }
            tm.createTimetable(wk, std::move(tt));
        }
    }

} // namespace Persistence
