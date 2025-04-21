#include "LecturerManager.hpp"
#include <iostream>
#include <algorithm>
#include <stdexcept>

// Add
void LecturerManager::add(std::unique_ptr<Lecturer> lec) {
    if (find(lec->getUserID()))
        throw std::runtime_error("Lecturer already exists");
    store.push_back(std::move(lec));
}

// Remove
void LecturerManager::remove(const std::string& id) {
    auto it = std::remove_if(store.begin(), store.end(),
        [&](const auto& ptr) {
            return ptr->getUserID() == id;
        });
    if (it == store.end())
        throw std::runtime_error("Lecturer ID not found");
    store.erase(it, store.end());
}

// Find
Lecturer* LecturerManager::find(const std::string& id) const {
    auto it = std::find_if(store.begin(), store.end(),
        [&](const auto& ptr) {
            return ptr->getUserID() == id;
        });
    return (it == store.end() ? nullptr : it->get());
}

// List
void LecturerManager::list() const {
    for (const auto& lec : store)
        lec->displayUserInfo();
}
