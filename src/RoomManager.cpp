#include "RoomManager.hpp"

RoomManager::RoomManager() : rooms() {}

void RoomManager::addRoom(const Room& room) {
    auto it = std::find_if(rooms.begin(), rooms.end(),
        [&room](const Room& r) { return r.getRoomId() == room.getRoomId(); });
    if (it != rooms.end()) {
        throw std::runtime_error("Room with ID " + room.getRoomId() + " already exists.");
    }
    rooms.push_back(room);
}

void RoomManager::updateRoom(const std::string& roomId, const Room& updatedRoom) {
    auto it = std::find_if(rooms.begin(), rooms.end(),
        [&roomId](const Room& r) { return r.getRoomId() == roomId; });
    if (it != rooms.end()) {
        *it = updatedRoom;
    }
    else {
        throw std::runtime_error("Room with ID " + roomId + " not found.");
    }
}

void RoomManager::deleteRoom(const std::string& roomId) {
    auto it = std::remove_if(rooms.begin(), rooms.end(),
        [&roomId](const Room& r) { return r.getRoomId() == roomId; });
    if (it != rooms.end()) {
        rooms.erase(it, rooms.end());
    }
    else {
        throw std::runtime_error("Room with ID " + roomId + " not found.");
    }
}

void RoomManager::listRooms() const {
    if (rooms.empty()) {
        std::cout << "No rooms available." << std::endl;
        return;
    }
    for (const auto& r : rooms) {
        r.displayRoom();
        std::cout << "-----------------" << std::endl;
    }
}
