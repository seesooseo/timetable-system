#ifndef MODULE_HPP
#define MODULE_HPP

#include <string>
#include <iostream>
#include <vector>
#include <string>


/*
 * Class: Module
 * -------------------------------
 * Represents an academic module with a unique code, a name, and a description.
 * Provides basic CRUD functionality through getters and setters, as well as a 
 * method to display the module information.
 */
class Module {
private:
    std::string moduleCode;   // Unique code for the module (e.g., "COMP1234")
    std::string moduleName;   // Name of the module (e.g., "Introduction to Programming")
    std::string description;  // Brief description of the module content
    std::vector<std::string> lecturerIds;

public:
    // Default constructor for an empty Module object.
    Module();

    // Parameterized constructor to create a module with specified attributes.
    Module(const std::string &code, const std::string &name, const std::string &desc);

    // Copy constructor.
    Module(const Module &other);

    // Destructor.
    ~Module();

    // Getter for moduleCode.
   const std::string& getModuleCode() const;

    // Setter for moduleCode.
    void setModuleCode(const std::string &code);
    
    // Getter for moduleName.
    const std::string& getModuleName() const;

    // Setter for moduleName.
    void setModuleName(const std::string &name);
    
    // Getter for description.
    const std::string& getDescription() const;

    // Setter for description.
    void setDescription(const std::string &desc);
    
    // Display module details to the console.
    void displayModule() const;

    void addLecturer(const std::string& lid) {
        lecturerIds.push_back(lid);
     
    }

};

#endif // MODULE_HPP
