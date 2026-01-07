/*
 * FileManager.cpp - File persistence class implementation
 * Part A: Traffic Light Management System
 */

#include "FileManager.h"
#include "Admin.h"
#include "Operator.h"

FileManager::FileManager(const std::string& dir) : dataDir(dir) {
    // Create data directory if it doesn't exist (Windows compatible)
    #ifdef _WIN32
        system(("if not exist \"" + dataDir + "\" mkdir \"" + dataDir + "\"").c_str());
    #else
        system(("mkdir -p " + dataDir).c_str());
    #endif
}

void FileManager::saveIntersections(const std::vector<Intersection>& intersections) {
    std::ofstream file(dataDir + "intersections.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not save intersections.\n";
        return;
    }
    
    // Write header
    file << "id,name,green,yellow,red,operator\n";
    
    // Write data
    for (const auto& i : intersections) {
        file << i.getId() << ","
             << i.getName() << ","
             << i.getGreenDuration() << ","
             << i.getYellowDuration() << ","
             << i.getRedDuration() << ","
             << i.getAssignedOperator() << "\n";
    }
    
    file.close();
    std::cout << "Intersections saved to " << dataDir << "intersections.csv\n";
}

std::vector<Intersection> FileManager::loadIntersections() {
    std::vector<Intersection> intersections;
    std::ifstream file(dataDir + "intersections.csv");
    
    if (!file.is_open()) {
        return intersections;  // Return empty if file doesn't exist
    }
    
    std::string line;
    std::getline(file, line);  // Skip header
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, name, opId;
        int green, yellow, red;
        
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        
        std::string temp;
        std::getline(ss, temp, ',');
        green = std::stoi(temp);
        std::getline(ss, temp, ',');
        yellow = std::stoi(temp);
        std::getline(ss, temp, ',');
        red = std::stoi(temp);
        std::getline(ss, opId);
        
        Intersection intersection(id, name, green, yellow, red);
        if (!opId.empty()) {
            intersection.assignOperator(opId);
        }
        intersections.push_back(intersection);
    }
    
    if (!intersections.empty()) {
        std::cout << "Loaded " << intersections.size() << " intersection(s) from file.\n";
    }
    return intersections;
}

void FileManager::saveUsers(const std::vector<Admin>& admins, const std::vector<Operator>& operators) {
    std::ofstream file(dataDir + "users.csv");
    if (!file.is_open()) {
        std::cerr << "Error: Could not save users.\n";
        return;
    }
    
    // Write header
    file << "id,username,password,role,assigned_intersections\n";
    
    // Write admins
    for (const auto& admin : admins) {
        file << admin.getId() << ","
             << admin.getUsername() << ","
             << admin.getPassword() << ","
             << "ADMIN" << ","
             << "\n";
    }
    
    // Write operators
    for (const auto& op : operators) {
        file << op.getId() << ","
             << op.getUsername() << ","
             << op.getPassword() << ","
             << "OPERATOR" << ",";
        
        // Save assigned intersections as semicolon-separated list
        auto assigned = op.getAssignedIntersections();
        for (size_t i = 0; i < assigned.size(); i++) {
            file << assigned[i];
            if (i < assigned.size() - 1) file << ";";
        }
        file << "\n";
    }
    
    file.close();
    std::cout << "Users saved to " << dataDir << "users.csv\n";
}

void FileManager::loadUsers(std::vector<Admin>& admins, std::vector<Operator>& operators) {
    std::ifstream file(dataDir + "users.csv");
    
    if (!file.is_open()) {
        return;  // File doesn't exist
    }
    
    std::string line;
    std::getline(file, line);  // Skip header
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string id, username, password, role, assignedStr;
        
        std::getline(ss, id, ',');
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');
        std::getline(ss, role, ',');
        std::getline(ss, assignedStr);
        
        if (role == "ADMIN") {
            admins.emplace_back(id, username, password);
        } else if (role == "OPERATOR") {
            Operator op(id, username, password);
            
            // Parse assigned intersections
            if (!assignedStr.empty()) {
                std::stringstream assignedSS(assignedStr);
                std::string intId;
                while (std::getline(assignedSS, intId, ';')) {
                    if (!intId.empty()) {
                        op.addAssignedIntersection(intId);
                    }
                }
            }
            operators.push_back(op);
        }
    }
    
    if (!admins.empty() || !operators.empty()) {
        std::cout << "Loaded " << admins.size() << " admin(s) and " 
                  << operators.size() << " operator(s) from file.\n";
    }
}
