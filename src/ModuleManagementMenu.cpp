// ModuleManagementMenu.cpp
#include "ModuleManagementMenu.hpp"

void moduleManagementMenu(ModuleManager& moduleManager) {
    int choice = 0;
    do {
        std::cout << "\n--- Module Management Menu ---" << std::endl;
        std::cout << "1. Add Module" << std::endl;
        std::cout << "2. Update Module" << std::endl;
        std::cout << "3. Delete Module" << std::endl;
        std::cout << "4. Search Module" << std::endl;
        std::cout << "5. List Modules" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // Clear newline

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
                std::cout << "Module added successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
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
                std::cout << "Module updated successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string code;
            std::cout << "\nEnter module code to delete: ";
            std::getline(std::cin, code);
            try {
                moduleManager.deleteModule(code);
                std::cout << "Module deleted successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 4: {
            std::string code;
            std::cout << "\nEnter module code to search: ";
            std::getline(std::cin, code);
            try {
                Module mod = moduleManager.searchModule(code);
                std::cout << "Module found:" << std::endl;
                mod.displayModule();
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 5: {
            std::cout << "\nListing all modules:" << std::endl;
            moduleManager.listModules();
            break;
        }
        case 0:
            std::cout << "Returning to Admin Menu..." << std::endl;
            break;
        default:
            std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 0);
}
