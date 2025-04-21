#include "Admin.hpp"

// Default constructor initializes an Admin with empty id and name.
Admin::Admin() : User() {
    // Additional initializations for Admin-specific attributes can go here.
}

// Parameterized constructor initializes an Admin with the provided id and name.
Admin::Admin(const std::string &id, const std::string &name) : User(id, name) {
    // Additional initialization as needed.
}

// Virtual destructor ensures proper cleanup for derived classes.
Admin::~Admin() {
    // Cleanup code if needed.
}

// Override displayUserInfo to provide Admin-specific details.
void Admin::displayUserInfo() const {
    std::cout << "Admin User Information:" << std::endl;
    std::cout << "User ID: " << getUserID() << std::endl;
    std::cout << "Name: " << getName() << std::endl;
    // You can display more admin-specific details here if needed.
}
