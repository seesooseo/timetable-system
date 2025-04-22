#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <iostream>
#include <string>

class Room {
private:
    std::string roomId;
    std::string roomName; // E.g., "John Clare Lecture Theatre 006"
public:
    Room() : roomId(""), roomName("") {}
    Room(const std::string& id, const std::string& name) : roomId(id), roomName(name) {}

    const std::string& getRoomId() const { return roomId; }
    const std::string& getRoomName() const { return roomName; }

    void displayRoom() const {
        std::cout << "Room ID: " << roomId << "\nRoom Name: " << roomName << std::endl;
    }
};

#endif // ROOM_HPP
