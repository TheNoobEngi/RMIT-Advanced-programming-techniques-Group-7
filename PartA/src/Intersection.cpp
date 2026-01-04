/*
 * Intersection.cpp - Intersection class implementation
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 2] - Traffic Light Control
 * Key Logic: Only ONE direction can be GREEN at any time
 */

#include "Intersection.h"

Intersection::Intersection() 
    : id(""), name(""), greenDuration(30), yellowDuration(5), redDuration(25),
      currentGreenIndex(0), countdown(30), autoMode(true), 
      currentPhase(LightState::GREEN), assignedOperatorId("") {
    lights[0] = TrafficLight("North");
    lights[1] = TrafficLight("South");
    lights[2] = TrafficLight("East");
    lights[3] = TrafficLight("West");
    setGreenDirection(0);
}

Intersection::Intersection(const std::string& id, const std::string& name,
                           int green, int yellow, int red)
    : id(id), name(name), greenDuration(green), yellowDuration(yellow), 
      redDuration(red), currentGreenIndex(0), countdown(green), 
      autoMode(true), currentPhase(LightState::GREEN), assignedOperatorId("") {
    lights[0] = TrafficLight("North");
    lights[1] = TrafficLight("South");
    lights[2] = TrafficLight("East");
    lights[3] = TrafficLight("West");
    setGreenDirection(0);
}

// Getters
std::string Intersection::getId() const { return id; }
std::string Intersection::getName() const { return name; }
bool Intersection::isAutoMode() const { return autoMode; }
int Intersection::getCountdown() const { return countdown; }
std::string Intersection::getAssignedOperator() const { return assignedOperatorId; }
int Intersection::getGreenDuration() const { return greenDuration; }
int Intersection::getYellowDuration() const { return yellowDuration; }
int Intersection::getRedDuration() const { return redDuration; }

// Setters
void Intersection::setId(const std::string& newId) { id = newId; }
void Intersection::setName(const std::string& newName) { name = newName; }
void Intersection::setDurations(int green, int yellow, int red) {
    greenDuration = green;
    yellowDuration = yellow;
    redDuration = red;
}
void Intersection::assignOperator(const std::string& opId) { assignedOperatorId = opId; }

// CORE LOGIC: Set green direction
// Ensures only ONE light is GREEN at a time
void Intersection::setGreenDirection(int newIndex) {
    if (newIndex < 0 || newIndex > 3) return;
    
    // Set ALL lights to RED first
    for (int i = 0; i < 4; i++) {
        lights[i].setState(LightState::RED);
    }
    
    // Set the new direction to GREEN
    lights[newIndex].setState(LightState::GREEN);
    currentGreenIndex = newIndex;
    currentPhase = LightState::GREEN;
    countdown = greenDuration;
    
    // Log the state change
    Logger::getInstance().log(name, lights[newIndex].getDirection(), 
                              "GREEN", !autoMode);
}

// AUTOMATIC CYCLING LOGIC
// Called every second to advance the cycle
void Intersection::tick() {
    if (!autoMode) return;
    
    countdown--;
    
    if (countdown <= 0) {
        if (currentPhase == LightState::GREEN) {
            // GREEN -> YELLOW
            lights[currentGreenIndex].setState(LightState::YELLOW);
            currentPhase = LightState::YELLOW;
            countdown = yellowDuration;
            
            Logger::getInstance().log(name, lights[currentGreenIndex].getDirection(),
                                      "YELLOW", false);
        }
        else if (currentPhase == LightState::YELLOW) {
            // YELLOW -> RED, then next direction goes GREEN
            lights[currentGreenIndex].setState(LightState::RED);
            
            Logger::getInstance().log(name, lights[currentGreenIndex].getDirection(),
                                      "RED", false);
            
            // Move to next direction
            int nextIndex = (currentGreenIndex + 1) % 4;
            setGreenDirection(nextIndex);
        }
    }
}

// MODE CONTROL
void Intersection::toggleMode() {
    autoMode = !autoMode;
    std::cout << "Mode switched to: " << (autoMode ? "AUTO" : "MANUAL") << "\n";
}

void Intersection::setAutoMode(bool mode) {
    autoMode = mode;
}

// MANUAL OVERRIDE
// Allows operator to set any direction to GREEN
void Intersection::manualOverride(int direction) {
    if (autoMode) {
        std::cout << "Cannot override in AUTO mode. Switch to MANUAL first.\n";
        return;
    }
    
    if (direction < 0 || direction > 3) {
        std::cout << "Invalid direction. Use 0-3 (N, S, E, W).\n";
        return;
    }
    
    // First set current green to yellow briefly
    lights[currentGreenIndex].setState(LightState::YELLOW);
    Logger::getInstance().log(name, lights[currentGreenIndex].getDirection(),
                              "YELLOW", true);
    
    // Then set new direction to green
    for (int i = 0; i < 4; i++) {
        lights[i].setState(LightState::RED);
    }
    lights[direction].setState(LightState::GREEN);
    currentGreenIndex = direction;
    currentPhase = LightState::GREEN;
    
    Logger::getInstance().log(name, lights[direction].getDirection(),
                              "GREEN", true);
    
    std::cout << "Manually set " << DIRECTIONS[direction] << " to GREEN.\n";
}

// DISPLAY STATUS
void Intersection::displayStatus() const {
    std::cout << "\n=== " << name << " (" << id << ") ===\n";
    std::cout << "Mode: " << (autoMode ? "AUTO" : "MANUAL") << "\n";
    std::cout << "Current State:\n";
    
    for (int i = 0; i < 4; i++) {
        std::cout << "  " << std::setw(6) << lights[i].getDirection() << ": "
                  << std::setw(6) << lights[i].getStateString();
        if (lights[i].getState() == LightState::GREEN) {
            std::cout << " (" << countdown << "s)";
        }
        std::cout << "\n";
    }
    
    if (autoMode && currentPhase == LightState::GREEN) {
        std::cout << "\nNext change in " << countdown << " seconds...\n";
    }
}

std::string Intersection::getCompactStatus() const {
    std::ostringstream oss;
    oss << "N:" << lights[0].getDisplayChar() 
        << " S:" << lights[1].getDisplayChar()
        << " E:" << lights[2].getDisplayChar()
        << " W:" << lights[3].getDisplayChar();
    return oss.str();
}
