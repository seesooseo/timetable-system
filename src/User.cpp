#include "User.hpp"

// Default constructor initializes an empty user.
User::User() : userID(""), name("") {
}

// Parameterized constructor initializes the user with the provided ID and name.
User::User(const std::string &id, const std::string &name) : userID(id), name(name) {
}

// Virtual destructor to allow safe deletion of derived objects.
User::~User() {
    // Cleanup tasks if necessary.
}

// Return the user's ID.
const std::string& User::getUserID() const {
    return userID;
}

// Set or update the user's ID.
void User::setUserID(const std::string &id) {
    userID = id;
}

// Return the user's name.
const std::string& User::getName() const {
    return name;
}

// Set or update the user's name.
void User::setName(const std::string &name) {
    this->name = name;
}
