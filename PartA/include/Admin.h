/*
 * Admin.h - Admin class header
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: INHERITANCE from User, POLYMORPHISM via virtual methods
 */

#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include "Intersection.h"

// Forward declaration
class Operator;

class Admin : public User {
public:
    Admin();
    Admin(const std::string& id, const std::string& uname, const std::string& pwd);
    
    std::string getRole() const override;
    void displayMenu() override;
    
    // Admin functionality
    void manageIntersections(std::vector<Intersection>& intersections,
                              std::vector<Operator>& operators);
    void addIntersection(std::vector<Intersection>& intersections);
    void removeIntersection(std::vector<Intersection>& intersections);
    void viewAndEditIntersection(std::vector<Intersection>& intersections,
                                  std::vector<Operator>& operators);
    void configureIntersection(std::vector<Intersection>& intersections);
    void assignOperatorToIntersection(std::vector<Intersection>& intersections,
                                      std::vector<Operator>& operators);
    void viewAllLogs();
    void listIntersections(const std::vector<Intersection>& intersections);
};

#endif
