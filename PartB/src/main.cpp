/*
 * Part B: Electric Motor Control and Simulation
 * Smart City Systems - Group Project
 * 
 * This module controls motor speed through PWM, reads ADC feedback,
 * and implements closed-loop control with the ousbMotorSim simulator.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include "MotorSimulator.h"
#include "Logger.h"
#include "FeedbackController.h"

using namespace std;

// Clear screen (cross-platform)
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Pause and wait for user input
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Display main menu
void displayMenu() {
    cout << "\n========================================" << endl;
    cout << "   ELECTRIC MOTOR CONTROL SYSTEM" << endl;
    cout << "       Part B - Smart City" << endl;
    cout << "========================================" << endl;
    cout << "1. View Current Status" << endl;
    cout << "2. Set Target ADC Value" << endl;
    cout << "3. Set PWM Manually" << endl;
    cout << "4. Read ADC Value" << endl;
    cout << "5. Start Feedback Control Loop" << endl;
    cout << "6. Configure Controller Settings" << endl;
    cout << "7. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Choice: ";
}

// Display current motor status
void displayStatus(MotorSimulator& motor, int targetADC) {
    cout << "\n---------- MOTOR STATUS ----------" << endl;
    cout << "Connection: " << (motor.isConnected() ? "CONNECTED" : "DISCONNECTED") << endl;
    
    if (motor.isConnected()) {
        int pwm = motor.getPWM();
        int adc = motor.readADC(0);
        
        cout << "Current PWM: " << pwm << "%" << endl;
        cout << "Current ADC: " << adc << endl;
        cout << "Target ADC:  " << targetADC << endl;
        
        if (adc >= 0) {
            int error = targetADC - adc;
            cout << "Error:       " << (error >= 0 ? "+" : "") << error << endl;
        }
    }
    cout << "----------------------------------" << endl;
}

int main() {
    // Initialize components
    MotorSimulator motor("ousbMotorSim.exe");
    Logger logger("motor_log.csv");
    FeedbackController controller(motor, logger);
    
    // Try to connect to simulator
    clearScreen();
    cout << "========================================" << endl;
    cout << "   ELECTRIC MOTOR CONTROL SYSTEM" << endl;
    cout << "       Part B - Smart City" << endl;
    cout << "========================================\n" << endl;
    
    cout << "Connecting to motor simulator..." << endl;
    
    if (motor.connect()) {
        cout << "[OK] Successfully connected to ousbMotorSim.exe" << endl;
    } else {
        cout << "[WARNING] Could not connect to simulator." << endl;
        cout << "Some features may not work properly." << endl;
    }
    
    // Open log file
    if (logger.open()) {
        cout << "[OK] Log file opened: " << logger.getFilename() << endl;
    } else {
        cout << "[WARNING] Could not open log file." << endl;
    }
    
    pauseScreen();
    
    int choice;
    bool running = true;
    
    while (running) {
        clearScreen();
        displayMenu();
        
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        switch (choice) {
            case 1: {
                // View current status
                displayStatus(motor, controller.getTarget());
                pauseScreen();
                break;
            }
            
            case 2: {
                // Set target ADC
                cout << "\nCurrent target: " << controller.getTarget() << endl;
                cout << "Enter new target ADC value (0-1023): ";
                int target;
                cin >> target;
                
                if (target >= 0 && target <= 1023) {
                    controller.setTarget(target);
                    cout << "Target ADC set to: " << target << endl;
                } else {
                    cout << "Invalid value. Must be between 0 and 1023." << endl;
                }
                pauseScreen();
                break;
            }
            
            case 3: {
                // Set PWM manually
                if (!motor.isConnected()) {
                    cout << "\n[ERROR] Not connected to simulator!" << endl;
                    pauseScreen();
                    break;
                }
                
                cout << "\nCurrent PWM: " << motor.getPWM() << "%" << endl;
                cout << "Enter new PWM duty cycle (0-100%): ";
                int pwm;
                cin >> pwm;
                
                if (motor.setPWM(pwm)) {
                    cout << "PWM set to: " << pwm << "%" << endl;
                    
                    // Log manual change
                    int adc = motor.readADC(0);
                    int error = controller.getTarget() - adc;
                    logger.log(0, pwm, adc, error);
                } else {
                    cout << "[ERROR] Failed to set PWM!" << endl;
                }
                pauseScreen();
                break;
            }
            
            case 4: {
                // Read ADC
                if (!motor.isConnected()) {
                    cout << "\n[ERROR] Not connected to simulator!" << endl;
                    pauseScreen();
                    break;
                }
                
                cout << "\n--- ADC Readings ---" << endl;
                cout << "ADC Channel 0: " << motor.readADC(0) << endl;
                cout << "ADC Channel 5: " << motor.readADC(5) << endl;
                cout << "ADC Channel 6: " << motor.readADC(6) << endl;
                pauseScreen();
                break;
            }
            
            case 5: {
                // Start feedback control
                if (!motor.isConnected()) {
                    cout << "\n[ERROR] Not connected to simulator!" << endl;
                    cout << "Please ensure ousbMotorSim.exe is accessible." << endl;
                    pauseScreen();
                    break;
                }
                
                clearScreen();
                int result = controller.run();
                
                if (result >= 0) {
                    cout << "\nLog saved to: " << logger.getFilename() << endl;
                }
                pauseScreen();
                break;
            }
            
            case 6: {
                // Configure controller
                cout << "\n--- CONTROLLER SETTINGS ---" << endl;
                cout << "1. Target ADC:      " << controller.getTarget() << endl;
                cout << "2. Error Threshold: +/-" << controller.getThreshold() << endl;
                cout << "3. Proportional Gain (Kp): " << controller.getGain() << endl;
                cout << "\nSelect setting to change (1-3, or 0 to cancel): ";
                
                int setting;
                cin >> setting;
                
                switch (setting) {
                    case 1: {
                        cout << "Enter new target ADC (0-1023): ";
                        int val;
                        cin >> val;
                        if (val >= 0 && val <= 1023) controller.setTarget(val);
                        break;
                    }
                    case 2: {
                        cout << "Enter new error threshold: ";
                        int val;
                        cin >> val;
                        if (val > 0) controller.setThreshold(val);
                        break;
                    }
                    case 3: {
                        cout << "Enter new Kp gain (e.g., 0.1): ";
                        double val;
                        cin >> val;
                        if (val > 0) controller.setGain(val);
                        break;
                    }
                }
                
                cout << "Settings updated." << endl;
                pauseScreen();
                break;
            }
            
            case 7: {
                // Exit
                running = false;
                cout << "\nClosing motor control system..." << endl;
                cout << "Log saved to: " << logger.getFilename() << endl;
                cout << "Goodbye!" << endl;
                break;
            }
            
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
        }
    }
    
    logger.close();
    return 0;
}
