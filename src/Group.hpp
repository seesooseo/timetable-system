#ifndef GROUP_HPP
#define GROUP_HPP

#include <string>
#include <iostream>

class Group {
private:
    std::string groupId;
    std::string groupName;
public:
    // Constructors
    Group() : groupId(""), groupName("") {}
    Group(const std::string& id, const std::string& name) : groupId(id), groupName(name) {}

    // Getters
    const std::string& getGroupId() const { return groupId; }
    const std::string& getGroupName() const { return groupName; }

    // Display method for group details
    void displayGroup() const {
        std::cout << "Group ID: " << groupId << "\nGroup Name: " << groupName << std::endl;
    }
};

#endif // GROUP_HPP
