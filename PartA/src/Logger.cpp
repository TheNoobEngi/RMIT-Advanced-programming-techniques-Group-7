/*
 * Logger.cpp - Activity logging class implementation
 * Part A: Traffic Light Management System
 */

#include "Logger.h"
#include <conio.h>

Logger::Logger() : logFilePath("data/traffic_log.txt") {}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(const std::string& intersection, const std::string& direction,
                 const std::string& newState, bool isManual) {
    std::ofstream file(logFilePath, std::ios::app);
    if (file.is_open()) {
        file << getCurrentTimestamp() << ","
             << intersection << ","
             << direction << ","
             << newState << ","
             << (isManual ? "MANUAL" : "AUTO") << "\n";
        file.close();
    }
}

std::vector<std::string> Logger::getAllLogs() {
    std::vector<std::string> logs;
    std::ifstream file(logFilePath);
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            logs.push_back(line);
        }
    }
    return logs;
}

void Logger::displayLogs() {
    auto logs = getAllLogs();
    if (logs.empty()) {
        std::cout << "\nNo activity logs found.\n";
        return;
    }
    
    const int logsPerPage = 20;
    int totalPages = (logs.size() + logsPerPage - 1) / logsPerPage;
    int currentPage = 0;
    bool viewing = true;
    
    while (viewing) {
        clearScreen();
        
        std::cout << "==================== ACTIVITY LOGS ====================\n";
        std::cout << "Page " << (currentPage + 1) << " of " << totalPages 
                  << " | Total: " << logs.size() << " entries\n";
        std::cout << "--------------------------------------------------------\n";
        std::cout << std::setw(10) << "Time" << " | "
                  << std::setw(20) << "Intersection" << " | "
                  << std::setw(8) << "Dir" << " | "
                  << std::setw(7) << "State" << " | "
                  << "Mode\n";
        std::cout << "--------------------------------------------------------\n";
        
        // Calculate start and end indices for current page (reversed order)
        int totalLogs = logs.size();
        int startIdx = totalLogs - 1 - (currentPage * logsPerPage);
        int endIdx = std::max(startIdx - logsPerPage + 1, 0);
        
        for (int i = startIdx; i >= endIdx; i--) {
            // Parse CSV line
            std::stringstream ss(logs[i]);
            std::string time, intersection, direction, state, mode;
            std::getline(ss, time, ',');
            std::getline(ss, intersection, ',');
            std::getline(ss, direction, ',');
            std::getline(ss, state, ',');
            std::getline(ss, mode, ',');
            
            // Truncate intersection name if too long
            if (intersection.length() > 20) {
                intersection = intersection.substr(0, 17) + "...";
            }
            
            std::cout << std::setw(10) << time << " | "
                      << std::setw(20) << intersection << " | "
                      << std::setw(8) << direction << " | "
                      << std::setw(7) << state << " | "
                      << mode << "\n";
        }
        
        std::cout << "--------------------------------------------------------\n";
        std::cout << "[A] Previous Page  [D] Next Page  [Q] Exit\n";
        std::cout << "--------------------------------------------------------\n";
        
        // Wait for key press
        char key = _getch();
        
        switch (key) {
            case 'a':
            case 'A':
                if (currentPage > 0) {
                    currentPage--;
                }
                break;
            case 'd':
            case 'D':
                if (currentPage < totalPages - 1) {
                    currentPage++;
                }
                break;
            case 'q':
            case 'Q':
                viewing = false;
                break;
        }
    }
}
