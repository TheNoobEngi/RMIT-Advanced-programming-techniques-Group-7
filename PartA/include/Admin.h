/*
 * Admin.h - Admin class header
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 1 TODO] - Implement Admin functionality
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
    
    // TODO: Implement all admin functions
    void addIntersection(std::vector<Intersection>& intersections);
    void removeIntersection(std::vector<Intersection>& intersections);
    void configureIntersection(std::vector<Intersection>& intersections);
    void assignOperatorToIntersection(std::vector<Intersection>& intersections,
                                      std::vector<Operator>& operators);
    void viewAllLogs();
    void listIntersections(const std::vector<Intersection>& intersections);
};

#endif
