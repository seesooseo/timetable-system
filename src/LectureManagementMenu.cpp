// LecturerManagementMenu.cpp
#include "LecturerManagementMenu.hpp"
#include <iostream>
#include <string>

void lecturerManagementMenu(LecturerManager& lm) {
    int c{};
    do {
        std::cout << "\n--- Lecturer Menu ---\n"
            << "1 Add  2 Remove  3 List  0 Back\n> ";
        std::cin >> c; std::cin.ignore();
        try {
            if (c == 1) {
                std::string id, name;
                std::cout << "ID: "; std::getline(std::cin, id);
                std::cout << "Name: "; std::getline(std::cin, name);
                lm.add(std::make_unique<Lecturer>(id, name));
            }
            else if (c == 2) {
                std::string id; std::cout << "ID: "; std::getline(std::cin, id);
                lm.remove(id);
            }
            else if (c == 3) lm.list();
        }
        catch (std::exception& e) { std::cout << e.what() << '\n'; }
    } while (c);
}
