/*
 * FileManager.h - File persistence class header
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 1 TODO] - Implement save/load functionality
 */

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Intersection.h"

// Forward declarations
class Admin;
class Operator;

class FileManager {
private:
    std::string dataDir;
    
public:
    FileManager(const std::string& dir = "data/");
    
    // TODO: Save intersections to CSV
    void saveIntersections(const std::vector<Intersection>& intersections);
    
    // TODO: Load intersections from CSV
    std::vector<Intersection> loadIntersections();
    
    // TODO: Save users to CSV
    void saveUsers(const std::vector<Admin>& admins, const std::vector<Operator>& operators);
    
    // TODO: Load users from CSV
    void loadUsers(std::vector<Admin>& admins, std::vector<Operator>& operators);
};

#endif
