// ModuleManagementMenu.cpp
#include "ModuleManagementMenu.hpp"
#include "InputUtility.hpp"    // << include the helper

void moduleManagementMenu(ModuleManager& moduleManager) {
    int choice;
    do {
        std::cout << "\n--- Module Management Menu ---\n";
        std::cout << "1. Add Module\n";
        std::cout << "2. Update Module\n";
        std::cout << "3. Delete Module\n";
        std::cout << "4. Search Module\n";
        std::cout << "5. List Modules\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← replace raw cin with our helper to guard non‑numeric & out‑of‑range
        choice = utils::readIntInRange("Enter your choice", 0, 5);

        switch (choice) {
        case 1: {
            std::string code, name, desc;
            std::cout << "\nEnter module code: ";
            std::getline(std::cin, code);
            std::cout << "Enter module name: ";
            std::getline(std::cin, name);
            std::cout << "Enter module description: ";
            std::getline(std::cin, desc);
            try {
                moduleManager.addModule(Module(code, name, desc));
                std::cout << "Module added successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 2: {
            std::string code, name, desc;
            std::cout << "\nEnter module code to update: ";
            std::getline(std::cin, code);
            std::cout << "Enter new module name: ";
            std::getline(std::cin, name);
            std::cout << "Enter new module description: ";
            std::getline(std::cin, desc);
            try {
                moduleManager.updateModule(code, Module(code, name, desc));
                std::cout << "Module updated successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::string code;
            std::cout << "\nEnter module code to delete: ";
            std::getline(std::cin, code);
            try {
                moduleManager.deleteModule(code);
                std::cout << "Module deleted successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 4: {
            std::string code;
            std::cout << "\nEnter module code to search: ";
            std::getline(std::cin, code);
            try {
                Module mod = moduleManager.searchModule(code);
                std::cout << "Module found:\n";
                mod.displayModule();
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 5:
            std::cout << "\nListing all modules:\n";
            moduleManager.listModules();
            break;
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
