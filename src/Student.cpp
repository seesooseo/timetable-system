#include "Student.hpp"

// Default constructor simply calls the default constructor of User.
Student::Student() : User() {
    // Additional initialization can be added here if needed.
}

// Parameterized constructor calls the base class constructor to set id and name.
Student::Student(const std::string& id, const std::string& name, const std::string& gid) : User(id, name), groupId(gid) {
    // Additional student-specific initialization can be done here.
}

// Destructor
Student::~Student() {
    // Any necessary cleanup (none needed here).
}

// Displays the student’s information.
void Student::displayUserInfo() const {
    std::cout << "Student User Information:" << std::endl;
    std::cout << "User ID: " << getUserID() << std::endl;
    std::cout << "Name: " << getName() << std::endl;
}
