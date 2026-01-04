/*
 * Operator.cpp - Operator class implementation
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 2] - Operator Role
 * Demonstrates: INHERITANCE and POLYMORPHISM
 */

#include "Operator.h"

Operator::Operator() : User() {}

Operator::Operator(const std::string& id, const std::string& uname, const std::string& pwd)
    : User(id, uname, pwd) {}

std::string Operator::getRole() const { 
    return "OPERATOR"; 
}

void Operator::displayMenu() {
    std::cout << "\n=== Operator Menu ===\n";
    std::cout << "1. View Assigned Intersections\n";
    std::cout << "2. Monitor Intersection (Live View)\n";
    std::cout << "3. Switch Mode (AUTO/MANUAL)\n";
    std::cout << "4. Manual Override\n";
    std::cout << "5. Logout\n";
    std::cout << "Choice: ";
}

void Operator::addAssignedIntersection(const std::string& intId) {
    assignedIntersectionIds.push_back(intId);
}

bool Operator::isAssignedTo(const std::string& intId) const {
    for (const auto& id : assignedIntersectionIds) {
        if (id == intId) return true;
    }
    return false;
}

std::vector<std::string> Operator::getAssignedIntersections() const {
    return assignedIntersectionIds;
}

void Operator::viewAssignedIntersections(const std::vector<Intersection>& allIntersections) {
    std::cout << "\n=== Your Assigned Intersections ===\n";
    
    bool found = false;
    for (const auto& intersection : allIntersections) {
        if (isAssignedTo(intersection.getId())) {
            std::cout << "- " << intersection.getName() 
                      << " (" << intersection.getId() << ")"
                      << " | " << intersection.getCompactStatus()
                      << " | Mode: " << (intersection.isAutoMode() ? "AUTO" : "MANUAL")
                      << "\n";
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No intersections assigned to you.\n";
    }
}

// Live monitoring with auto-refresh
void Operator::monitorIntersection(Intersection& intersection) {
    std::cout << "\n=== Live Monitoring: " << intersection.getName() << " ===\n";
    std::cout << "(Press Ctrl+C to stop, or wait for timeout)\n\n";
    
    for (int i = 0; i < 30; i++) {  // Monitor for 30 seconds
        clearScreen();
        std::cout << "=== Live Monitoring: " << intersection.getName() << " ===\n";
        std::cout << "Time: " << getCurrentTimestamp() << "\n";
        
        intersection.displayStatus();
        
        // Tick the intersection (advance time)
        intersection.tick();
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\nMonitoring ended.\n";
}

void Operator::switchMode(Intersection& intersection) {
    intersection.toggleMode();
}

void Operator::performOverride(Intersection& intersection) {
    if (intersection.isAutoMode()) {
        std::cout << "Switch to MANUAL mode first!\n";
        return;
    }
    
    std::cout << "\nSelect direction to set GREEN:\n";
    std::cout << "0. North\n";
    std::cout << "1. South\n";
    std::cout << "2. East\n";
    std::cout << "3. West\n";
    std::cout << "Choice: ";
    
    int direction;
    std::cin >> direction;
    
    intersection.manualOverride(direction);
}
