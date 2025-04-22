#include "Module.hpp"

// Default constructor initializes an empty module.
Module::Module() : moduleCode(""), moduleName(""), description("") {
}

// Parameterized constructor to initialize a module with given values.
Module::Module(const std::string &code, const std::string &name, const std::string &desc)
    : moduleCode(code), moduleName(name), description(desc) {
}

// Copy constructor creates a new module as a copy of an existing one.
Module::Module(const Module& other)
    : moduleCode(other.moduleCode)
    , moduleName(other.moduleName)
    , description(other.description)
    , lecturerIds(other.lecturerIds)    // ← copy this too
{
}

// Destructor; no dynamic memory, so no special cleanup is required.
Module::~Module() {}

// Get the module's unique code.
const std::string& Module::getModuleCode() const {
    return moduleCode;
}

// Set or update the module's code.
void Module::setModuleCode(const std::string &code) {
    moduleCode = code;
}

// Get the module's name.
const std::string& Module::getModuleName() const {
    return moduleName;
}

// Set or update the module's name.
void Module::setModuleName(const std::string &name) {
    moduleName = name;
}

// Get the description of the module.
const std::string& Module::getDescription() const {
    return description;
}

// Set or update the module's description.
void Module::setDescription(const std::string &desc) {
    description = desc;
}

// Display the module information on the console.
void Module::displayModule() const {
    std::cout << "Module Code: " << moduleCode << std::endl;
    std::cout << "Module Name: " << moduleName << std::endl;
    std::cout << "Description: " << description << std::endl;
}
