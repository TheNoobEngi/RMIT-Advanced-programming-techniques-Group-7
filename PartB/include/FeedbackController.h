/*
 * FeedbackController.h - Closed-Loop Feedback Controller
 * Part B: Electric Motor Control and Simulation
 */

#ifndef FEEDBACK_CONTROLLER_H
#define FEEDBACK_CONTROLLER_H

#include "MotorSimulator.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <conio.h>  // For _kbhit() and _getch()

class FeedbackController {
private:
    MotorSimulator& motor;
    Logger& logger;
    
    int targetADC;
    int errorThreshold;
    int maxIterations;
    double kp;  // Proportional gain

public:
    FeedbackController(MotorSimulator& m, Logger& l)
        : motor(m), logger(l), targetADC(500), errorThreshold(5), 
          maxIterations(50), kp(0.1) {}

    // Set target ADC value
    void setTarget(int target) { 
        targetADC = target; 
    }

    // Set error threshold for stopping
    void setThreshold(int threshold) { 
        errorThreshold = threshold; 
    }

    // Set max iterations
    void setMaxIterations(int max) { 
        maxIterations = max; 
    }

    // Set proportional gain
    void setGain(double gain) { 
        kp = gain; 
    }

    // Get current settings
    int getTarget() const { return targetADC; }
    int getThreshold() const { return errorThreshold; }
    double getGain() const { return kp; }

    // Run feedback control loop
    // Returns: final PWM value, or -1 if failed
    int run() {
        if (!motor.isConnected()) {
            std::cerr << "Error: Motor simulator not connected!" << std::endl;
            return -1;
        }

        std::cout << "\n========================================" << std::endl;
        std::cout << "     FEEDBACK CONTROL LOOP STARTED" << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Target ADC = " << targetADC << std::endl;
        std::cout << "Error Threshold = +/-" << errorThreshold << std::endl;
        std::cout << "Max Iterations = " << maxIterations << std::endl;
        std::cout << "Proportional Gain = " << kp << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Press [Q] at any time to stop early" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Starting feedback control...\n" << std::endl;

        int pwm = motor.getPWM();
        if (pwm < 0) pwm = 50;  // Default starting PWM
        
        int lastADC = -1;
        int stableCount = 0;
        const int stableThreshold = 3;  // How many stable readings before considering settled

        for (int iteration = 1; iteration <= maxIterations; iteration++) {
            // Check for keyboard interrupt (Q to quit)
            if (_kbhit()) {
                char key = _getch();
                if (key == 'q' || key == 'Q') {
                    std::cout << "\n[USER STOPPED] Control loop terminated by user." << std::endl;
                    std::cout << "Final PWM = " << pwm << "%" << std::endl;
                    return pwm;
                }
            }

            // Set PWM first, then wait for motor to respond
            if (!motor.setPWM(pwm)) {
                std::cerr << "\n[ERROR] Failed to set PWM! Communication error." << std::endl;
                return -1;
            }
            
            // Wait for ADC to stabilize (motor needs time to respond to PWM change)
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // Read ADC
            int adc = motor.readADC(0);
            if (adc < 0) {
                std::cerr << "\n[ERROR] ADC read failed! Communication error." << std::endl;
                std::cerr << "Safely terminating control loop..." << std::endl;
                return -1;
            }

            // Calculate error
            int error = targetADC - adc;

            // Log data
            logger.log(iteration, pwm, adc, error);

            // Display iteration info
            std::cout << "[Loop " << iteration << "/" << maxIterations << "] PWM = " << pwm << "%, ADC = " << adc 
                      << ", Error = " << (error >= 0 ? "+" : "") << error << std::endl;

            // Check if target reached
            if (abs(error) <= errorThreshold) {
                std::cout << "\n========================================" << std::endl;
                std::cout << "  TARGET REACHED!" << std::endl;
                std::cout << "  Final PWM = " << pwm << "%" << std::endl;
                std::cout << "  Final ADC = " << adc << std::endl;
                std::cout << "  Final Error = " << error << std::endl;
                std::cout << "========================================" << std::endl;
                return pwm;
            }
            
            // Check if ADC is no longer changing (motor has settled but can't reach target)
            if (adc == lastADC) {
                stableCount++;
                if (stableCount >= stableThreshold) {
                    std::cout << "\n[INFO] Motor appears to have settled." << std::endl;
                    std::cout << "ADC stable at " << adc << " for " << stableCount << " readings." << std::endl;
                    
                    // If at PWM limits and still can't reach target, stop
                    if ((pwm >= 100 && error > 0) || (pwm <= 0 && error < 0)) {
                        std::cout << "[WARNING] Cannot reach target - PWM at limit." << std::endl;
                        break;
                    }
                }
            } else {
                stableCount = 0;
            }
            lastADC = adc;

            // Proportional control: adjust PWM based on error
            int adjustment = static_cast<int>(kp * error);
            if (adjustment == 0 && error != 0) {
                adjustment = (error > 0) ? 1 : -1;  // Minimum adjustment
            }
            pwm += adjustment;

            // Clamp PWM to valid range
            if (pwm > 100) pwm = 100;
            if (pwm < 0) pwm = 0;
        }

        std::cout << "\n[WARNING] Max iterations (" << maxIterations << ") reached without achieving target." << std::endl;
        std::cout << "Final PWM = " << pwm << "%" << std::endl;
        return pwm;
    }
};

#endif // FEEDBACK_CONTROLLER_H

