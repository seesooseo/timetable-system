// RoomManagementMenu.cpp
#include "RoomManagementMenu.hpp"
#include "InputUtility.hpp"    // ← include the helper
#include <iostream>
#include <string>

void roomManagementMenu(RoomManager& roomManager) {
    int choice;
    do {
        std::cout << "\n--- Room Management Menu ---\n";
        std::cout << "1. Add Room\n";
        std::cout << "2. Update Room\n";
        std::cout << "3. Delete Room\n";
        std::cout << "4. List Rooms\n";
        std::cout << "0. Return to Admin Menu\n";

        // ← safely read 0–4
        choice = utils::readIntInRange("Enter your choice", 0, 4);

        switch (choice) {
        case 1: {
            std::string id, name;
            std::cout << "\nEnter room ID: ";
            std::getline(std::cin, id);
            std::cout << "Enter room name: ";
            std::getline(std::cin, name);
            try {
                roomManager.addRoom(Room(id, name));
                std::cout << "Room added successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
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
                std::cout << "Room updated successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "\nEnter room ID to delete: ";
            std::getline(std::cin, id);
            try {
                roomManager.deleteRoom(id);
                std::cout << "Room deleted successfully.\n";
            }
            catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
            }
            break;
        }
        case 4:
            std::cout << "\nListing all rooms:\n";
            roomManager.listRooms();
            break;
        case 0:
            std::cout << "Returning to Admin Menu...\n";
            break;
        }
    } while (choice != 0);
}
