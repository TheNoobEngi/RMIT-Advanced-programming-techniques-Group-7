/*
 * FileManager.h - File persistence class header
 * Part A: Traffic Light Management System
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
    
    // Intersection persistence
    void saveIntersections(const std::vector<Intersection>& intersections);
    std::vector<Intersection> loadIntersections();
    
    // User persistence
    void saveUsers(const std::vector<Admin>& admins, const std::vector<Operator>& operators);
    void loadUsers(std::vector<Admin>& admins, std::vector<Operator>& operators);
};

#endif
