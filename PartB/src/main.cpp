/*
 * Part B: Electric Motor Control and Simulation
 * Smart City Systems - Group Project
 * 
 * This module controls motor speed through PWM, reads ADC feedback,
 * and implements closed-loop control with the ousbMotorSim simulator.
 */

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;
    

class MotorSim {
    public:
        bool connect(); //Connect to ousbMotorSim.exe
        int readADC(); //Read ADC value
        bool setPWM(int dutycycle); //Send PWM command
};
class Log {
    std::ofstream logfile;        //Output file stream for logging
public:
    Log(const std::string& filename) { logfile.open(filename); }
    ~Log() { logfile.close(); }
    void log(int timestamp, int pwm, int adc, int error) {
        logfile << timestamp << "," << pwm << "," << adc << "," << error << std::endl;
    }
};
int main() {
    std::cout << "=== Electric Motor Control and Simulation ===" << std::endl;
    std::cout << "Part B - Smart City Systems" << std::endl;
    const int TARGET_ADC = 500;//The target ADC Value
    MotorSim sim;
    Log log("motion_log");
    int pwm = 45;
    int timestamp = 0;

    std::cout << "Target ADC = " << TARGET_ADC << std::endl;
    std::cout << "Starting feedback control..." << std::endl;

    if (!sim.connect()) {
    std::cerr << "Failed to connect to simulator. Terminating..." << std::endl;
    return 1;
}
    while (true) {
        int adc = sim.readADC();
        if (adc == -1) { // Invalid read
            std::cerr << "ADC read failed. Ending..." << std::endl;
            break;
        }
        int error = TARGET_ADC - adc;
        // Simple control logic: adjust PWM to minimise error
        pwm += error / 10; // Tuning factor
        if (pwm > 100) pwm = 100;
        if (pwm < 0) pwm = 0;
        sim.setPWM(pwm);
        log.log(timestamp, pwm, adc, error);
        std::cout << "Loop " << timestamp << "PWM = " << pwm
                  << "%, ADC = " << adc << ", Error = "
                  << (error > 0 ? "+" : "") << error << std::endl;
        if (abs(error) <= 5) break; // Target condition
        timestamp++;
        // Sleep or wait suitable time (e.g. std::this_thread::sleep_for)
    }
    std::cout << "Target reached! Final PWM = " << pwm << "%" << std::endl;
    std::cout << "Log saved to motor_log.csv" << std::endl;
    return 0;
}
