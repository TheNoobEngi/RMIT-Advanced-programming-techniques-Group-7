/*
 * User.cpp - Base User class implementation
 * Part A: Traffic Light Management System
 * 
 * [SHARED] - Base class for Admin and Operator
 */

#include "User.h"

User::User() : id(""), username(""), password("") {}

User::User(const std::string& id, const std::string& uname, const std::string& pwd)
    : id(id), username(uname), password(pwd) {}

std::string User::getId() const { return id; }
std::string User::getUsername() const { return username; }

bool User::authenticate(const std::string& pwd) const {
    return password == pwd;
}

void User::setId(const std::string& newId) { id = newId; }
void User::setUsername(const std::string& uname) { username = uname; }
void User::setPassword(const std::string& pwd) { password = pwd; }
