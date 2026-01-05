/*
 * Logger.h - Activity logging class header
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 1 TODO] - Implement logging functionality
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
    
    // TODO: Implement logging to file
    // Format: timestamp,intersection,direction,state,mode
    // Example: 10:05:32,District 7,North,GREEN,AUTO
    void log(const std::string& intersection, const std::string& direction,
             const std::string& newState, bool isManual = false);
    
    // TODO: Read all logs from file
    std::vector<std::string> getAllLogs();
    
    // TODO: Display logs in formatted table
    void displayLogs();
};

#endif
