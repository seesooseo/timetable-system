#ifndef STUDENT_HPP
#define STUDENT_HPP

#include "User.hpp"
#include <iostream>
#include <string>

/*
 * Class: Student
 * -------------------------------
 * Derived from the abstract base class User.
 * Represents a student user in the timetabling system.
 */
class Student : public User {
    std::string groupId;
public:
    // Default constructor
    Student();

    // Parameterized constructor to initialize a student with an ID and name.
    Student(const std::string& id, const std::string& name, const std::string& gid = "");

    void  setGroup(const std::string& gid) { groupId = gid; }
    const std::string& getGroup() const { return groupId; }

    // Destructor
    ~Student() override;

    // Override displayUserInfo to show student-specific information.
    virtual void displayUserInfo() const override;
};

#endif // STUDENT_HPP
