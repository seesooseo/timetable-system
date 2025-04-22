#include "TimeValidation.hpp"
#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>


// Function to validate session times
void getValidSessionTimes(const int& start, const int& end) {
    if (start < 0 || end < 0 || start > end) {
        throw std::runtime_error("Invalid session times: start must be less than or equal to end and non-negative.");
    }
}

// Helper function to validate input as time
bool isValidTimeInput(const std::string& input) {
    try {
        int m = std::stoi(input); // Try converting input to integer
        return (m >= 0 && m < 24*60);
    }
    catch (...) {
        return false;
    }
}

// Function to get valid start and end time inputs
void getValidTimeInput(int& start, int& end) {
    std::cout << "[DEBUG] getValidTimeInput CALLED!\n";
    std::string input;
    bool valid = false;

    // Loop until valid input is entered
    while (!valid) {
        std::cout << "Enter start time (in minutes): ";
        std::getline(std::cin, input);
        if (isValidTimeInput(input)) {
            start = std::stoi(input); // Convert string to integer
            valid = true;
        }
        else {
            std::cout << "Invalid start time input. Please enter a valid time.\n";
        }
    }

    valid = false;
    while (!valid) {
        std::cout << "Enter end time (in minutes): ";
        std::getline(std::cin, input);
        if (isValidTimeInput(input)) {
            end = std::stoi(input); // Convert string to integer
            valid = true;
        }
        else {
            std::cout << "Invalid end time input. Please enter a valid time.\n";
        }
    }

    // Validate the session times (start should be less than or equal to end)
    try {
        getValidSessionTimes(start, end);
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
        getValidTimeInput(start, end); // Recursively ask for valid input
    }
}

void getValidTimeInput(const int& start, const int& end) {
    std::string input;
    bool valid = false;

    // Loop until valid input is entered for start time
    while (!valid) {
        std::cout << "Enter start time (in minutes): ";
        std::getline(std::cin, input);
        if (isValidTimeInput(input)) {
            // You cannot modify the const reference, so we don't modify start or end.
            std::cout << "Start time is valid.\n";
            valid = true;
        }
        else {
            std::cout << "Invalid start time input. Please enter a valid time.\n";
        }
    }

    valid = false;
    // Loop until valid input is entered for end time
    while (!valid) {
        std::cout << "Enter end time (in minutes): ";
        std::getline(std::cin, input);
        if (isValidTimeInput(input)) {
            // You cannot modify the const reference, so we don't modify start or end.
            std::cout << "End time is valid.\n";
            valid = true;
        }
        else {
            std::cout << "Invalid end time input. Please enter a valid time.\n";
        }
    }

    // You cannot modify start and end directly here since they're const references,
    // so we just validate the existing values and ensure they are within acceptable ranges.
    try {
        getValidSessionTimes(start, end); // Validate the session times
    }
    catch (const std::runtime_error& e) {
        std::cout << e.what() << "\n";
        getValidTimeInput(start, end); // Recursively ask for valid input if exception is thrown
    }
}

