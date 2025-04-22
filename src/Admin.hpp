#ifndef ADMIN_HPP
#define ADMIN_HPP

#include "User.hpp"
#include <iostream>

/*
 * Class: Admin
 * -------------------------------
 * Derived from the User class.
 * Represents an administrative user in the timetabling system.
 * This class is intended to provide functionalities specific to admin tasks,
 * such as managing modules, groups, sessions, rooms, and timetables.
 */
class Admin : public User {
public:
    // Default constructor
    Admin();

    // Parameterized constructor to initialize admin with an ID and name.
    Admin(const std::string &id, const std::string &name);

    // Destructor
    ~Admin() override;

    // Override the pure virtual function to display admin information.
    virtual void displayUserInfo() const override;

    // Additional admin-specific methods can be declared here.
    // For example:
    // void manageModules();
};

#endif // ADMIN_HPP
