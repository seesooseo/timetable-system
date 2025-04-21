#ifndef LECTURERMANAGER_HPP
#define LECTURERMANAGER_HPP

#include "Lecturer.hpp"
#include <vector>
#include <memory>
#include <string>

class LecturerManager {
private:
    // <-- make sure this name matches your .cpp
    std::vector<std::unique_ptr<Lecturer>> store;

public:
    // Adds a new Lecturer (throws if duplicate ID)
    void add(std::unique_ptr<Lecturer> lec);

    // Removes by ID (throws if not found)
    void remove(const std::string& id);

    // Finds by ID, or nullptr if not present
    Lecturer* find(const std::string& id) const;

    // Lists all lecturers to stdout
    void list() const;
};

#endif // LECTURERMANAGER_HPP

