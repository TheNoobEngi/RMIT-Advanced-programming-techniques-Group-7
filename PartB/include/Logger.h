/*
 * Logger.h - CSV Data Logger
 * Part B: Electric Motor Control and Simulation
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>

class Logger {
private:
    std::ofstream logFile;
    std::string filename;
    bool isOpen;

public:
    Logger(const std::string& fname = "motor_log.csv") 
        : filename(fname), isOpen(false) {}

    ~Logger() {
        close();
    }

    // Open log file and write header
    bool open() {
        logFile.open(filename);
        if (logFile.is_open()) {
            logFile << "Time,PWM(%),ADC,Error" << std::endl;
            isOpen = true;
            return true;
        }
        return false;
    }

    // Log a single iteration
    void log(int timestamp, int pwm, int adc, int error) {
        if (!isOpen) return;
        logFile << timestamp << "," << pwm << "," << adc << "," << error << std::endl;
        logFile.flush();
    }

    // Log with actual timestamp
    void logWithTime(int pwm, int adc, int error) {
        if (!isOpen) return;
        
        auto now = std::time(nullptr);
        auto tm = std::localtime(&now);
        
        logFile << std::put_time(tm, "%H:%M:%S") << ","
                << pwm << "," << adc << "," << error << std::endl;
        logFile.flush();
    }

    // Close the log file
    void close() {
        if (isOpen) {
            logFile.close();
            isOpen = false;
        }
    }

    // Get filename
    std::string getFilename() const { return filename; }

    // Check if open
    bool isOpened() const { return isOpen; }
};

#endif // LOGGER_H
