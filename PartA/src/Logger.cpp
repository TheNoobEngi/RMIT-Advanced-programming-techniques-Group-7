/*
 * Logger.cpp - Activity logging class implementation
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 1 TODO] - Implement all logging functionality
 */

#include "Logger.h"

Logger::Logger() : logFilePath("data/traffic_log.txt") {}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& intersection, const std::string& direction,
                 const std::string& newState, bool isManual) {
    // TODO: Log to traffic_log.txt
}

std::vector<std::string> Logger::getAllLogs() {
    // TODO: Read logs from file and return as vector
    return {};
}

void Logger::displayLogs() {
    // TODO: Display all logs with headers
}
