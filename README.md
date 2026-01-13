# Smart City Systems - Group Project

**Course:** Advanced Programming Techniques  
**University:** RMIT University  
**Group:** 7

---

## Project Overview

This group project integrates two C++ simulation modules demonstrating object-oriented programming (OOP), system design, data persistence, and real-time simulation in a smart city context.

---

## Project Structure

```
├── PartA/
│   ├── include/
│   │   ├── Utils.h
│   │   ├── TrafficLight.h
│   │   ├── Intersection.h
│   │   ├── User.h
│   │   ├── Operator.h
│   │   ├── Admin.h
│   │   ├── Logger.h
│   │   ├── FileManager.h
│   │   └── platform.h
│   ├── src/
│   │   ├── main.cpp
│   │   ├── Utils.cpp
│   │   ├── TrafficLight.cpp
│   │   ├── Intersection.cpp
│   │   ├── User.cpp
│   │   ├── Operator.cpp
│   │   ├── Admin.cpp
│   │   ├── Logger.cpp
│   │   └── FileManager.cpp
│   ├── data/
│   │   ├── traffic_log.txt
│   │   └── intersections.csv
│   │   └── users.csv
│   └── traffic.exe               # Executable file
│
├── PartB/
│   ├── include/
│   │   ├── MotorSimulator.h
│   │   ├── Logger.h
│   │   └── FeedbackController.h
│   ├── src/
│   │   ├── main.cpp
│   │   ├── motor_log.csv
│   │   └── ousbMotorSim.exe
│   └── motor.exe                 # Executable file
│
└── README.md
```

---

## Part A: Traffic Light Management System

A console-based application that simulates a multi-intersection traffic light management system supporting automatic and manual operation modes.

### Features
- **Admin Role:** Create/configure intersections, manage timings, assign operators
- **Operator Role:** Monitor intersections, switch modes, manual override
- **Traffic Light Control:** Automatic cycling (G→Y→R), one-GREEN-at-a-time rule
- **Data Logging:** All state changes logged with timestamps
- **File Persistence:** Save/load configurations and logs

### Team Responsibilities
| Member          | Focus Areas |
|-----------------|-------------|
| Chang Chao-Yuan | Admin Role, Data Logging & File Persistence |
| Dao The Anh     | Operator Role, Traffic Light Control Logic |

---

## Part B: Electric Motor Control and Simulation

A console-based program that interacts with a motor simulator (ousbMotorSim.exe) to control motor speed through PWM and implement closed-loop control.

### Features
- **PWM Control:** Set/modify frequency and duty cycle (0-100%)
- **ADC Feedback:** Read analog sensor values (channels 0, 5, 6)
- **Feedback Loop:** Auto-adjust PWM to maintain target ADC value
- **Data Logging:** Log timestamp, PWM, ADC, and error to CSV
- **Exception Handling:** Graceful simulator failure handling

### Team Responsibilities
| Member | Focus Areas |
|--------|-------------|
| Nguyen Duc Khoi | PWM Control, Feedback Control Loop |
| John Falconer | ADC Feedback Reading, Data Logging & Exception Handling |

---

## OOP Principles Demonstrated

- **Encapsulation:** Private members with public interfaces
- **Abstraction:** Abstract base classes with pure virtual methods
- **Inheritance:** Admin/Operator inherit from User base class
- **Polymorphism:** Virtual methods for role-based behavior

---

## How to Compile

```bash
# Part A - compile and run from PartA folder
cd PartA
g++ -o traffic.exe -Iinclude src/*.cpp
./traffic.exe

# Part B - compile and run from PartB folder
cd PartB
g++ -o motor.exe -Iinclude src/main.cpp
cd src
../motor.exe
```

---

## Test Credentials (Part A)

| Role | Username | Password |
|------|----------|----------|
| Admin | admin | admin123 |
| Operator | operator1 | op123 |
