/*
 * Logger.h - Activity logging class header
 * Part A: Traffic Light Management System
 */

#ifndef LOGGER_H
#define LOGGER_H

#include "Utils.h"

class Logger {
private:
    std::string logFilePath;
    Logger();
    
public:
    static Logger& getInstance();
    
    // Log a traffic light state change
    void log(const std::string& intersection, const std::string& direction,
             const std::string& newState, bool isManual = false);
    
    // Get all log entries
    std::vector<std::string> getAllLogs();
    
    // Display formatted log table
    void displayLogs();
};

#endif
