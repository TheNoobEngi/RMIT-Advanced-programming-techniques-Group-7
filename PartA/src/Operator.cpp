/*
 * Operator.cpp - Operator class implementation
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: INHERITANCE and POLYMORPHISM
 */

#include "Operator.h"
#include <conio.h>  // For _kbhit() and _getch() on Windows

Operator::Operator() : User() {}

Operator::Operator(const std::string& id, const std::string& uname, const std::string& pwd)
    : User(id, uname, pwd) {}

std::string Operator::getRole() const { 
    return "OPERATOR"; 
}

void Operator::displayMenu() {
    std::cout << "\n============ OPERATOR MENU ============\n";
    std::cout << "1. Monitor Intersection (Live View)\n";
    std::cout << "2. Logout\n";
    std::cout << "=======================================\n";
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
    std::cout << "\n========== YOUR ASSIGNED INTERSECTIONS ==========\n";
    
    bool found = false;
    for (const auto& intersection : allIntersections) {
        if (isAssignedTo(intersection.getId())) {
            std::cout << "  " << intersection.getId() << " - " << intersection.getName() 
                      << "\n    Status: " << intersection.getCompactStatus()
                      << " | Mode: " << (intersection.isAutoMode() ? "AUTO" : "MANUAL")
                      << "\n";
            found = true;
        }
    }
    
    if (!found) {
        std::cout << "No intersections assigned to you.\n";
    }
    std::cout << "==================================================\n";
}

// Interactive live monitoring with inline controls
void Operator::monitorIntersection(Intersection& intersection) {
    bool monitoring = true;
    
    while (monitoring) {
        clearScreen();
        
        std::cout << "============================================================\n";
        std::cout << "              LIVE TRAFFIC MONITORING                       \n";
        std::cout << "============================================================\n\n";
        
        std::cout << "Intersection: " << intersection.getName() << " (" << intersection.getId() << ")\n";
        std::cout << "Time: " << getCurrentTimestamp() << "\n";
        std::cout << "Mode: " << (intersection.isAutoMode() ? "AUTO (cycling)" : "MANUAL (operator control)") << "\n";
        std::cout << "\n";
        
        // Display traffic light status
        intersection.displayStatus();
        
        std::cout << "\n------------------------------------------------------------\n";
        std::cout << "  CONTROLS:\n";
        std::cout << "  [Q] Exit monitoring\n";
        std::cout << "  [M] Toggle Mode (AUTO/MANUAL)\n";
        std::cout << "  [R] Reset timer (restart countdown)\n";
        std::cout << "  [0-3] Override light (0=North, 1=South, 2=East, 3=West)\n";
        std::cout << "------------------------------------------------------------\n";
        
        // Tick the intersection if in AUTO mode
        if (intersection.isAutoMode()) {
            intersection.tick();
        }
        
        // Check for keyboard input (non-blocking)
        if (_kbhit()) {
            char key = _getch();
            
            switch (key) {
                case 'q':
                case 'Q':
                    monitoring = false;
                    std::cout << "\nExiting monitor...\n";
                    break;
                    
                case 'm':
                case 'M':
                    intersection.toggleMode();
                    std::cout << "\nMode switched to: " << (intersection.isAutoMode() ? "AUTO" : "MANUAL") << "\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    break;
                    
                case 'r':
                case 'R':
                    intersection.resetTimer();
                    std::cout << "\nTimer reset!\n";
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    break;
                    
                case '0':
                case '1':
                case '2':
                case '3': {
                    int direction = key - '0';
                    
                    // Remember current mode
                    bool wasAuto = intersection.isAutoMode();
                    
                    // Temporarily switch to manual if needed
                    if (wasAuto) {
                        intersection.setAutoMode(false);
                    }
                    
                    // Perform the override
                    intersection.manualOverride(direction);
                    
                    // Restore original mode
                    if (wasAuto) {
                        intersection.setAutoMode(true);
                    }
                    
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    break;
                }
                    
                default:
                    // Ignore other keys
                    break;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "\nMonitoring ended. Returning to menu...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void Operator::switchMode(Intersection& intersection) {
    std::cout << "\n=== MODE SWITCH ===\n";
    std::cout << "Intersection: " << intersection.getName() << "\n";
    std::cout << "Current Mode: " << (intersection.isAutoMode() ? "AUTO" : "MANUAL") << "\n";
    
    intersection.toggleMode();
    
    std::cout << "\nNew Mode: " << (intersection.isAutoMode() ? "AUTO" : "MANUAL") << "\n";
}

void Operator::performOverride(Intersection& intersection) {
    std::cout << "\n=== MANUAL OVERRIDE ===\n";
    std::cout << "Intersection: " << intersection.getName() << "\n";
    std::cout << "Current Status: " << intersection.getCompactStatus() << "\n";
    
    std::cout << "\nSelect direction to set GREEN:\n";
    std::cout << "  0. North\n";
    std::cout << "  1. South\n";
    std::cout << "  2. East\n";
    std::cout << "  3. West\n";
    std::cout << "Choice (0-3): ";
    
    int direction;
    std::cin >> direction;
    
    if (direction >= 0 && direction <= 3) {
        bool wasAuto = intersection.isAutoMode();
        if (wasAuto) intersection.setAutoMode(false);
        
        intersection.manualOverride(direction);
        
        if (wasAuto) intersection.setAutoMode(true);
        
        std::cout << "\nNew Status: " << intersection.getCompactStatus() << "\n";
    } else {
        std::cout << "\n[ERROR] Invalid direction. Please enter 0-3.\n";
    }
}
