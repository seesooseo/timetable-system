#include "GroupManager.hpp"

GroupManager::GroupManager() : groups() {}

void GroupManager::addGroup(const Group& group) {
    auto it = std::find_if(groups.begin(), groups.end(),
        [&group](const Group& g) { return g.getGroupId() == group.getGroupId(); });
    if (it != groups.end()) {
        throw std::runtime_error("Group with ID " + group.getGroupId() + " already exists.");
    }
    groups.push_back(group);
}

void GroupManager::updateGroup(const std::string& groupId, const Group& updatedGroup) {
    auto it = std::find_if(groups.begin(), groups.end(),
        [&groupId](const Group& g) { return g.getGroupId() == groupId; });
    if (it != groups.end()) {
        *it = updatedGroup;
    }
    else {
        throw std::runtime_error("Group with ID " + groupId + " not found.");
    }
}

void GroupManager::deleteGroup(const std::string& groupId) {
    auto it = std::remove_if(groups.begin(), groups.end(),
        [&groupId](const Group& g) { return g.getGroupId() == groupId; });
    if (it != groups.end()) {
        groups.erase(it, groups.end());
    }
    else {
        throw std::runtime_error("Group with ID " + groupId + " not found.");
    }
}

void GroupManager::listGroups() const {
    if (groups.empty()) {
        std::cout << "No groups available." << std::endl;
        return;
    }
    for (const auto& g : groups) {
        g.displayGroup();
        std::cout << "-----------------" << std::endl;
    }
}

// mutable version
Group* GroupManager::findGroup(const std::string& groupId) {
    auto it = std::find_if(groups.begin(), groups.end(),
        [&groupId](const Group& g) { return g.getGroupId() == groupId; });
    return (it == groups.end() ? nullptr : &*it);
}

// const version
const Group* GroupManager::findGroup(const std::string& groupId) const {
    auto it = std::find_if(groups.begin(), groups.end(),
        [&groupId](const Group& g) { return g.getGroupId() == groupId; });
    return (it == groups.end() ? nullptr : &*it);
}
