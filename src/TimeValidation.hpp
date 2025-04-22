#ifndef TIME_VALID_HPP
#define TIME_VALID_HPP
#pragma once
#include <string>

// Function declaration for session time validation
void getValidSessionTimes(const int& start, const int& end);

// Helper functions for time input validation
bool isValidTimeInput(const std::string& input);

// Function to convert input time to valid session times
void getValidTimeInput(int& start, int& end);

void getValidTimeInput(const int& start, const int& end);

#endif // TIME_VALID_HPP