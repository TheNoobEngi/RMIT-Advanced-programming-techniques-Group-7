/*
 * FileManager.cpp - File persistence class implementation
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 1 TODO] - Implement all save/load functionality
 */

#include "FileManager.h"
#include "Admin.h"
#include "Operator.h"

FileManager::FileManager(const std::string& dir) : dataDir(dir) {
    system(("mkdir " + dataDir + " 2>nul").c_str());
}

// TODO: Save intersections to CSV
// Format: id,name,green,yellow,red,operator
void FileManager::saveIntersections(const std::vector<Intersection>& intersections) {
    // TODO: Save to data/intersections.csv
}

// TODO: Load intersections from CSV
std::vector<Intersection> FileManager::loadIntersections() {
    // TODO: Load from data/intersections.csv
    return {};
}

// TODO: Save users to CSV
void FileManager::saveUsers(const std::vector<Admin>& admins, const std::vector<Operator>& operators) {
    // TODO: Save to data/users.csv
}

// TODO: Load users from CSV
void FileManager::loadUsers(std::vector<Admin>& admins, std::vector<Operator>& operators) {
    // TODO: Load from data/users.csv
}
