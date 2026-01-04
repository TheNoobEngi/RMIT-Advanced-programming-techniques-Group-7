/*
 * TrafficLight.h - Traffic Light class header
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: ENCAPSULATION - private state with public interface
 */

#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "Utils.h"

class TrafficLight {
private:
    LightState state;
    std::string direction;  // "North", "South", "East", "West"
    
public:
    TrafficLight();
    TrafficLight(const std::string& dir);
    
    // Getters
    LightState getState() const;
    std::string getDirection() const;
    std::string getStateString() const;
    std::string getDisplayChar() const;
    
    // Setter
    void setState(LightState newState);
};

#endif
