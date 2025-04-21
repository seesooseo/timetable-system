#include "ModuleManager.hpp"
#include <algorithm>


ModuleManager::ModuleManager() : modules() {}

// Adds a new module.
void ModuleManager::addModule(const Module& module) {
    auto it = std::find_if(modules.begin(), modules.end(),
        [&module](const Module& m) { return m.getModuleCode() == module.getModuleCode(); });
    if (it != modules.end()) {
        throw std::runtime_error("Module with code " + module.getModuleCode() + " already exists.");
    }
    modules.push_back(module);
}

// Updates an existing module.
void ModuleManager::updateModule(const std::string& moduleCode, const Module& updatedModule) {
    auto it = std::find_if(modules.begin(), modules.end(),
        [&moduleCode](const Module& m) { return m.getModuleCode() == moduleCode; });
    if (it != modules.end()) {
        *it = updatedModule;
    }
    else {
        throw std::runtime_error("Module with code " + moduleCode + " not found.");
    }
}

// Deletes a module.
void ModuleManager::deleteModule(const std::string& moduleCode) {
    auto it = std::remove_if(modules.begin(), modules.end(),
        [&moduleCode](const Module& m) { return m.getModuleCode() == moduleCode; });
    if (it != modules.end()) {
        modules.erase(it, modules.end());
    }
    else {
        throw std::runtime_error("Module with code " + moduleCode + " not found. Cannot delete.");
    }
}

// Searches for a module.
Module ModuleManager::searchModule(const std::string& moduleCode) const {
    auto it = std::find_if(modules.begin(), modules.end(),
        [&moduleCode](const Module& m) { return m.getModuleCode() == moduleCode; });
    if (it != modules.end()) {
        return *it;
    }
    else {
        throw std::runtime_error("Module with code " + moduleCode + " not found.");
    }
}

// Lists all modules.
void ModuleManager::listModules() const {
    if (modules.empty()) {
        std::cout << "No modules available." << std::endl;
        return;
    }
    for (const auto& m : modules) {
        m.displayModule();
        std::cout << "-----------------" << std::endl;
    }
}

Module* ModuleManager::findModule(const std::string& moduleCode) {
    // 'modules' is your private std::vector<Module>
    auto it = std::find_if(
        modules.begin(), modules.end(),
        [&moduleCode](const Module& m) { return m.getModuleCode() == moduleCode; }
    );
    if (it != modules.end())
        return &*it;
    return nullptr;
}