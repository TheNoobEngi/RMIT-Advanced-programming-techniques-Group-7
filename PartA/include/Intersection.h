/*
 * Intersection.h - Intersection class header
 * Part A: Traffic Light Management System
 * 
 * Demonstrates: COMPOSITION - contains 4 TrafficLight objects
 * [MEMBER 2] - Traffic Light Control
 */

#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "TrafficLight.h"
#include "Logger.h"

class Intersection {
private:
    std::string id;
    std::string name;
    TrafficLight lights[4];  // 0=North, 1=South, 2=East, 3=West
    
    int greenDuration;
    int yellowDuration;
    int redDuration;
    
    int currentGreenIndex;
    int countdown;
    bool autoMode;
    LightState currentPhase;
    std::string assignedOperatorId;
    
public:
    Intersection();
    Intersection(const std::string& id, const std::string& name,
                 int green = 30, int yellow = 5, int red = 25);
    
    // Getters
    std::string getId() const;
    std::string getName() const;
    bool isAutoMode() const;
    int getCountdown() const;
    std::string getAssignedOperator() const;
    int getGreenDuration() const;
    int getYellowDuration() const;
    int getRedDuration() const;
    
    // Setters
    void setId(const std::string& newId);
    void setName(const std::string& newName);
    void setDurations(int green, int yellow, int red);
    void assignOperator(const std::string& opId);
    
    // Core logic
    void setGreenDirection(int newIndex);
    void tick();
    void toggleMode();
    void setAutoMode(bool mode);
    void manualOverride(int direction);
    
    // Display
    void displayStatus() const;
    std::string getCompactStatus() const;
};

#endif
