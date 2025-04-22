#ifndef GROUPMANAGER_HPP
#define GROUPMANAGER_HPP

#include "Group.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

class GroupManager {
private:
    std::vector<Group> groups;
public:
    GroupManager();

    // CRUD functions for Group
    void addGroup(const Group& group);
    void updateGroup(const std::string& groupId, const Group& updatedGroup);
    void deleteGroup(const std::string& groupId);
    void listGroups() const;

    // new: lookup (returns nullptr if missing)
    Group * findGroup(const std::string & groupId);
    const Group * findGroup(const std::string & groupId) const;
    const std::vector<Group>& getGroups() const { return groups; }
};

#endif // GROUPMANAGER_HPP
