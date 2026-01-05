/*
 * Admin.cpp - Admin class implementation
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 1 TODO] - Implement all admin functionality
 */

#include "Admin.h"
#include "Operator.h"
#include "Logger.h"

Admin::Admin() : User() {}

Admin::Admin(const std::string& id, const std::string& uname, const std::string& pwd)
    : User(id, uname, pwd) {}

std::string Admin::getRole() const { 
    return "ADMIN"; 
}

void Admin::displayMenu() {
    std::cout << "\n=== Admin Menu ===\n";
    std::cout << "1. Add Intersection\n";
    std::cout << "2. Remove Intersection\n";
    std::cout << "3. Configure Intersection Timing\n";
    std::cout << "4. Assign Operator to Intersection\n";
    std::cout << "5. View All Activity Logs\n";
    std::cout << "6. List All Intersections\n";
    std::cout << "7. Logout\n";
    std::cout << "Choice: ";
}

// TODO: Add new intersection
void Admin::addIntersection(std::vector<Intersection>& intersections) {
    // TODO: Implement add intersection
}

// TODO: Remove intersection by ID
void Admin::removeIntersection(std::vector<Intersection>& intersections) {
    // TODO: Implement remove intersection
}

// TODO: Configure timing for an intersection
void Admin::configureIntersection(std::vector<Intersection>& intersections) {
    // TODO: Implement configure timing
}

// TODO: Assign operator to intersection
void Admin::assignOperatorToIntersection(std::vector<Intersection>& intersections,
                                         std::vector<Operator>& operators) {
    // TODO: Implement assign operator
}

// TODO: View all activity logs
void Admin::viewAllLogs() {
    // TODO: Call Logger::getInstance().displayLogs()
}

// TODO: List all intersections
void Admin::listIntersections(const std::vector<Intersection>& intersections) {
    // TODO: Display all intersections with their settings
}
