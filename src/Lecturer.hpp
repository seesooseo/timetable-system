#ifndef LECTURER_HPP
#define LECTURER_HPP

#include "User.hpp"
#include <vector>
#include <string>
#include <iostream>

class Lecturer : public User {
    std::vector<std::string> moduleCodes;

public:
    Lecturer() = default;
    Lecturer(const std::string& id, const std::string& name)
        : User(id, name) {
    }

    void addModule(const std::string& code) { moduleCodes.push_back(code); }
    const std::vector<std::string>& getModules() const { return moduleCodes; }

    void displayUserInfo() const override {
        std::cout << "Lecturer " << name << " (ID: " << userID << ")\nModules: ";
        if (moduleCodes.empty()) std::cout << "—";
        else for (const auto& m : moduleCodes) std::cout << m << ' ';
        std::cout << '\n';
    }
};

#endif