#include "RoomManagementMenu.hpp"
#include <iostream>
#include <string>

void roomManagementMenu(RoomManager& roomManager) {
    int choice = 0;
    do {
        std::cout << "\n--- Room Management Menu ---" << std::endl;
        std::cout << "1. Add Room" << std::endl;
        std::cout << "2. Update Room" << std::endl;
        std::cout << "3. Delete Room" << std::endl;
        std::cout << "4. List Rooms" << std::endl;
        std::cout << "0. Return to Admin Menu" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1: {
            std::string id, name;
            std::cout << "\nEnter room ID: ";
            std::getline(std::cin, id);
            std::cout << "Enter room name: ";
            std::getline(std::cin, name);
            try {
                roomManager.addRoom(Room(id, name));
                std::cout << "Room added successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 2: {
            std::string id, name;
            std::cout << "\nEnter room ID to update: ";
            std::getline(std::cin, id);
            std::cout << "Enter new room name: ";
            std::getline(std::cin, name);
            try {
                roomManager.updateRoom(id, Room(id, name));
                std::cout << "Room updated successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter room ID to delete: ";
            std::getline(std::cin, id);
            try {
                roomManager.deleteRoom(id);
                std::cout << "Room deleted successfully." << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "\nListing all rooms:" << std::endl;
            roomManager.listRooms();
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
