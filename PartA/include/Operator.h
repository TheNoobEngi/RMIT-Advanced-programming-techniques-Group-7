/*
 * Operator.h - Operator class header
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: INHERITANCE from User, POLYMORPHISM via virtual methods
 * [MEMBER 2] - Operator Role
 */

#ifndef OPERATOR_H
#define OPERATOR_H

#include "User.h"
#include "Intersection.h"

class Operator : public User {
private:
    std::vector<std::string> assignedIntersectionIds;
    
public:
    Operator();
    Operator(const std::string& id, const std::string& uname, const std::string& pwd);
    
    // Polymorphic methods
    std::string getRole() const override;
    void displayMenu() override;
    
    // Operator-specific methods
    void addAssignedIntersection(const std::string& intId);
    bool isAssignedTo(const std::string& intId) const;
    std::vector<std::string> getAssignedIntersections() const;
    
    void viewAssignedIntersections(const std::vector<Intersection>& allIntersections);
    void monitorIntersection(Intersection& intersection);
    void switchMode(Intersection& intersection);
    void performOverride(Intersection& intersection);
};

#endif
