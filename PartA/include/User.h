/*
 * User.h - Base User class header (Abstract)
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: ABSTRACTION with pure virtual methods
 * [SHARED] - Base class for Admin and Operator
 */

#ifndef USER_H
#define USER_H

#include "Utils.h"

class User {
protected:
    std::string id;
    std::string username;
    std::string password;
    
public:
    User();
    User(const std::string& id, const std::string& uname, const std::string& pwd);
    virtual ~User() = default;
    
    // Pure virtual methods - MUST be implemented by derived classes
    virtual std::string getRole() const = 0;
    virtual void displayMenu() = 0;
    
    // Common methods
    std::string getId() const;
    std::string getUsername() const;
    bool authenticate(const std::string& pwd) const;
    
    void setId(const std::string& newId);
    void setUsername(const std::string& uname);
    void setPassword(const std::string& pwd);
};

#endif
