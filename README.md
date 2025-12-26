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
├── PartA/                    # Traffic Light Management System
│   ├── src/
│   │   └── main.cpp          # Main source code
│   ├── include/              # Header files
│   └── data/                 # Data persistence files
│
├── PartB/                    # Electric Motor Control and Simulation
│   ├── src/
│   │   └── main.cpp          # Main source code
│   ├── include/              # Header files
│   └── data/                 # Data persistence files
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
- **PWM Control:** Set/modify frequency and duty cycle
- **ADC Feedback:** Read analog sensor values
- **Feedback Loop:** Auto-adjust PWM to maintain target ADC
- **Data Logging:** Log PWM, ADC, and error values
- **Exception Handling:** Graceful simulator failure handling

### Team Responsibilities
| Member | Focus Areas |
|--------|-------------|
| Member 3 | PWM Control, Feedback Control Loop |
| Member 4 | ADC Feedback Reading, Data Logging & Exception Handling |

---

## OOP Principles Demonstrated

- **Encapsulation:** Private members with public interfaces
- **Abstraction:** Abstract base classes with pure virtual methods
- **Inheritance:** Admin/Operator inherit from User base class
- **Polymorphism:** Virtual methods for role-based behavior

---

## How to Compile

```bash
g++ -o traffic.exe PartA/src/main.cpp
g++ -o motor.exe PartB/src/main.cpp
```

---

## Test Credentials (Part A)

| Role | Username | Password |
|------|----------|----------|
| Admin | admin | admin123 |
| Operator | operator1 | op123 |
