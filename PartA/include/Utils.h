/*
 * Utils.h - Utility functions and constants
 * Part A: Traffic Light Management System
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

// Light state enumeration
enum class LightState { RED, YELLOW, GREEN };

// Direction constants
const std::string DIRECTIONS[] = {"North", "South", "East", "West"};
const std::string SHORT_DIRS[] = {"N", "S", "E", "W"};

// Utility function declarations
std::string getCurrentTimestamp();
std::string lightStateToString(LightState state);
void clearScreen();
void pauseScreen();

#endif
