#ifndef SESSIONTYPE_HPP
#define SESSIONTYPE_HPP

#include <string>
#include <iostream>

class SessionType {
private:
    std::string typeId;
    std::string typeName; // e.g., Lecture, Lab
public:
    SessionType() : typeId(""), typeName("") {}
    SessionType(const std::string& id, const std::string& name) : typeId(id), typeName(name) {}

    std::string getTypeId() const { return typeId; }
    std::string getTypeName() const { return typeName; }

    void displaySessionType() const {
        std::cout << "Session Type ID: " << typeId << "\nSession Type: " << typeName << std::endl;
    }
};

#endif // SESSIONTYPE_HPP
