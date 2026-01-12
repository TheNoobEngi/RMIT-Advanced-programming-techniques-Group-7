/*
 * MotorSimulator.h - Interface to ousbMotorSim.exe
 * Part B: Electric Motor Control and Simulation
 */

#ifndef MOTOR_SIMULATOR_H
#define MOTOR_SIMULATOR_H

#include <string>
#include <cstdio>
#include <cstdlib>
#include <stdexcept>

class MotorSimulator {
private:
    std::string simulatorPath;
    bool connected;
    int currentPWM;
    int currentFreq;

    // Execute simulator command and return output
    std::string executeCommand(const std::string& args) {
        std::string command = simulatorPath + " " + args;
        char buffer[128];
        std::string result = "";
        
        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            connected = false;
            return "";
        }
        
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        
        int exitCode = _pclose(pipe);
        if (exitCode != 0) {
            // Check if it's a disconnect signal
            if (result.find("Disconnect") != std::string::npos || 
                result.find("Error") != std::string::npos) {
                connected = false;
            }
        }
        
        return result;
    }

    // Parse integer from output
    int parseValue(const std::string& output) {
        try {
            // Find first number in output
            size_t start = output.find_first_of("0123456789");
            if (start == std::string::npos) return -1;
            
            size_t end = output.find_first_not_of("0123456789", start);
            std::string numStr = output.substr(start, end - start);
            return std::stoi(numStr);
        } catch (...) {
            return -1;
        }
    }

public:
    MotorSimulator(const std::string& path = "ousbMotorSim.exe") 
        : simulatorPath(path), connected(false), currentPWM(0), currentFreq(1000) {}

    // Connect to the simulator
    bool connect() {
        std::string result = executeCommand("-r pwm 1");
        if (result.empty() || result.find("Error") != std::string::npos) {
            // Try without -r flag
            result = executeCommand("pwm 1");
        }
        
        if (!result.empty() && result.find("Disconnect") == std::string::npos) {
            connected = true;
            currentPWM = parseValue(result);
            if (currentPWM < 0) currentPWM = 0;
            return true;
        }
        return false;
    }

    // Check if connected
    bool isConnected() const { return connected; }

    // Read ADC value (channel 0, 5, or 6)
    int readADC(int channel = 0) {
        if (!connected) return -1;
        
        std::string result = executeCommand("-r ADC " + std::to_string(channel));
        if (result.empty()) {
            result = executeCommand("ADC " + std::to_string(channel));
        }
        
        int value = parseValue(result);
        if (value < 0 && !result.empty()) {
            // Sometimes the raw output is just the number
            try {
                value = std::stoi(result);
            } catch (...) {
                value = -1;
            }
        }
        return value;
    }

    // Set PWM duty cycle (0-255 for simulator, we'll convert from 0-100%)
    bool setPWM(int dutyCycle) {
        if (!connected) return false;
        
        // Clamp to 0-100
        if (dutyCycle < 0) dutyCycle = 0;
        if (dutyCycle > 100) dutyCycle = 100;
        
        // Convert percentage to 0-255 for simulator
        int simValue = (dutyCycle * 255) / 100;
        
        std::string result = executeCommand("-r pwm 1 " + std::to_string(simValue));
        if (result.empty()) {
            result = executeCommand("pwm 1 " + std::to_string(simValue));
        }
        
        if (!result.empty() && result.find("Error") == std::string::npos) {
            currentPWM = dutyCycle;
            return true;
        }
        return false;
    }

    // Get current PWM duty cycle
    int getPWM() {
        if (!connected) return -1;
        
        std::string result = executeCommand("-r pwm 1");
        if (result.empty()) {
            result = executeCommand("pwm 1");
        }
        
        int value = parseValue(result);
        if (value >= 0) {
            // Convert from 0-255 to percentage
            currentPWM = (value * 100) / 255;
        }
        return currentPWM;
    }

    // Set PWM frequency
    bool setPWMFreq(int freq) {
        if (!connected) return false;
        
        std::string result = executeCommand("-r pwm-freq 1 " + std::to_string(freq));
        if (result.empty()) {
            result = executeCommand("pwm-freq 1 " + std::to_string(freq));
        }
        
        if (!result.empty() && result.find("Error") == std::string::npos) {
            currentFreq = freq;
            return true;
        }
        return false;
    }

    // Get PWM frequency
    int getPWMFreq() {
        if (!connected) return -1;
        
        std::string result = executeCommand("-r pwm-freq 1");
        if (result.empty()) {
            result = executeCommand("pwm-freq 1");
        }
        
        int value = parseValue(result);
        if (value >= 0) {
            currentFreq = value;
        }
        return currentFreq;
    }

    // Simulate disconnect
    void disconnect() {
        executeCommand("-disconnect");
        connected = false;
    }
};

#endif // MOTOR_SIMULATOR_H
