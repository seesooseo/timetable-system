#pragma once
#include <string>

// Generates a full “sample.json” matching your schema (modules, rooms, groups, users, 
// timetables for weeks 1–26 with built‑in conflicts).  
// Call this once at startup if you need to bootstrap a JSON file.
void generateSampleJSON(const std::string& filename);
