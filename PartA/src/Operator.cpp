/*
 * Operator.cpp - Operator class implementation
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: INHERITANCE and POLYMORPHISM
 */

#include "Operator.h"
#include "platform.h"  // Cross-platform keyboard input

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
        if (!intersection.isAutoMode()) {
            std::cout << "  [R] Reset timer (restart countdown)\n";
            std::cout << "  [0-3] Override light (0=North, 1=South, 2=East, 3=West)\n";
        }
        std::cout << "------------------------------------------------------------\n";
        
        // Tick the intersection if in AUTO mode
        if (intersection.isAutoMode()) {
            intersection.tick();
        }
        
        // Check for keyboard input (non-blocking)
        if (KBHIT()) {
            char key = GETCH();
            
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
                    if (!intersection.isAutoMode()) {
                        intersection.resetTimer();
                        std::cout << "\nTimer reset!\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                    break;
                    
                case '0':
                case '1':
                case '2':
                case '3': {
                    if (intersection.isAutoMode()) {
                        std::cout << "\nCannot override in AUTO mode. Press [M] to switch to MANUAL first.\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                    } else {
                        int direction = key - '0';
                        intersection.manualOverride(direction);
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
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

