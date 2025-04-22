#ifndef USER_H
#define USER_H

#include <string>
#include <string>

/*
 * Abstract Class: User
 * -------------------------------
 * Serves as the base class for all user types (Admin, Student).
 * Contains common attributes and methods. It includes a pure virtual
 * function 'displayUserInfo' for polymorphic behavior in derived classes.
 */
class User {
protected:
    std::string userID;  // Unique identifier for the user.
    std::string name;    // The user's full name.

public:
    // Default constructor.
    User();

    // Parameterized constructor to initialize user with an ID and name.
    User(const std::string &id, const std::string &name);

    // Virtual destructor to ensure proper cleanup in derived classes.
    virtual ~User();

    // Getter for user ID.
    const std::string& getUserID() const;

    // Setter for user ID.
    void setUserID(const std::string &id);
    
    // Getter for user name.
    const std::string& getName() const;

    // Setter for user name.
    void setName(const std::string &name);
    
    // Pure virtual function to display user information.
    virtual void displayUserInfo() const = 0;
};

#endif // USER_H
