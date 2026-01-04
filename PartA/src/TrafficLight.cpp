/*
 * TrafficLight.cpp - Traffic Light class implementation
 * Part A: Traffic Light Management System
 * 
 * [MEMBER 2] - Operator Role, Traffic Light Control
 */

#include "TrafficLight.h"

TrafficLight::TrafficLight() : state(LightState::RED), direction("") {}

TrafficLight::TrafficLight(const std::string& dir) : state(LightState::RED), direction(dir) {}

LightState TrafficLight::getState() const { 
    return state; 
}

std::string TrafficLight::getDirection() const { 
    return direction; 
}

std::string TrafficLight::getStateString() const {
    return lightStateToString(state);
}

std::string TrafficLight::getDisplayChar() const {
    switch (state) {
        case LightState::GREEN: return "G";
        case LightState::YELLOW: return "Y";
        case LightState::RED: return "R";
        default: return "?";
    }
}

void TrafficLight::setState(LightState newState) {
    state = newState;
}
