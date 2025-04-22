#pragma once
#include <string>
#include <vector>
#include <utility>

namespace utils {
    /**
     * Prompt the user to enter an integer in the given range [min, max].
     * Repeats until valid input is provided.
     */
    int readIntInRange(const std::string& prompt, int min, int max);

    /**
     * Prompt the user to enter a unique non-empty string not found in 'existing'.
     * Repeats until valid input is provided.
     */
    std::string readUniqueString(const std::string& prompt, const std::vector<std::string>& existing);

    /**
     * Prompt the user to enter start and end times (in minutes since midnight).
     * Ensures 0 <= start < end <= 1440. Repeats until valid input is provided.
     * Returns a pair {start, end}.
     */
    std::pair<int, int> readTimeBounds(const std::string& prompt);
}


// src/utils/InputUtils.cpp
#include "InputUtility.hpp"
#include <iostream>
#include <limits>

namespace utils {

    inline int readIntInRange(const std::string& prompt, int min, int max) {
        int value;
        while (true) {
            std::cout << prompt << " (" << min << "-" << max << "): ";
            if (std::cin >> value && value >= min && value <= max) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
            std::cout << "Invalid input. Please enter an integer between " << min << " and " << max << ".\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    inline std::string readUniqueString(const std::string& prompt, const std::vector<std::string>& existing) {
        std::string input;
        while (true) {
            std::cout << prompt << ": ";
            std::getline(std::cin, input);
            bool duplicate = std::any_of(
                existing.begin(), existing.end(),
                [&](const auto& e) { return e == input; }
            );
            if (input.empty()) {
                std::cout << "Input cannot be empty.\n";
            }
            else if (duplicate) {
                std::cout << "Value already exists. Please enter a unique value.\n";
            }
            else {
                return input;
            }
        }
    }

    inline std::pair<int, int> readTimeBounds(const std::string& prompt) {
        int start, end;
        while (true) {
            std::cout << prompt << " start time in minutes since midnight: ";
            if (!(std::cin >> start)) goto fail;
            std::cout << prompt << " end time in minutes since midnight: ";
            if (!(std::cin >> end)) goto fail;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (start < 0 || start > 1440 || end < 0 || end > 1440 || start >= end) {
                std::cout << "Invalid time bounds. Ensure 0 <= start < end <= 1440.\n";
            }
            else {
                return { start, end };
            }
        fail:
            std::cout << "Invalid input. Please enter numeric minute values.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}