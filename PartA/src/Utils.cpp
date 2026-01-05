/*
 * Utils.cpp - Utility functions implementation
 * Part A: Traffic Light Management System
 */

#include "Utils.h"

std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_ptr = std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%H:%M:%S");
    return oss.str();
}

std::string lightStateToString(LightState state) {
    switch (state) {
        case LightState::GREEN: return "GREEN";
        case LightState::YELLOW: return "YELLOW";
        case LightState::RED: return "RED";
        default: return "UNKNOWN";
    }
}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
