#ifndef MODULEMANAGER_HPP
#define MODULEMANAGER_HPP

#include "Module.hpp"       // This header should define your Module class.
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <string>

class ModuleManager {
private:
    std::vector<Module> modules;
public:
    ModuleManager();
    void addModule(const Module& module);
    void updateModule(const std::string& moduleCode, const Module& updatedModule);
    void deleteModule(const std::string& moduleCode);
    Module searchModule(const std::string& moduleCode) const;
    void listModules() const;
    Module * findModule(const std::string & code);
};

#endif // MODULEMANAGER_HPP
