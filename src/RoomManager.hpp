#ifndef ROOMMANAGER_HPP
#define ROOMMANAGER_HPP

#include "Room.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iostream>

class RoomManager {
private:
    std::vector<Room> rooms;
public:
    RoomManager();

    // CRUD functions for Room
    void addRoom(const Room& room);
    void updateRoom(const std::string& roomId, const Room& updatedRoom);
    void deleteRoom(const std::string& roomId);
    void listRooms() const;
    const std::vector<Room>& getRooms() const { return rooms; }
};

#endif // ROOMMANAGER_HPP
