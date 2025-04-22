#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include <string>
#include "ModuleManager.hpp"
#include "RoomManager.hpp"
#include "SessionTypeManager.hpp"
#include "GroupManager.hpp"
#include "UserManager.hpp"
#include "LecturerManager.hpp"
#include "TimetableManager.hpp"

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
    );

    void loadAll(
        const std::string& filename,
        ModuleManager& mm,
        RoomManager& rm,
        SessionTypeManager& stm,
        GroupManager& gm,
        UserManager& um,
        LecturerManager& lm,
        TimetableManager& tm
    );
}

#endif // PERSISTENCE_HPP
