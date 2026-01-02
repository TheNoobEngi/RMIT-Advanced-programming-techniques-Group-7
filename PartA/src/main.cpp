/*
 * Part A: Traffic Light Management System
 * Smart City Systems - Group Project
 * 
 * Main entry point with user authentication and role-based menu routing.
 * 
 * OOP Principles Demonstrated:
 * - Encapsulation: Private members with public interfaces
 * - Abstraction: User base class with pure virtual methods
 * - Inheritance: Admin and Operator inherit from User
 * - Polymorphism: Virtual displayMenu() for different user types
 */

#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>

// ENUMS AND CONSTANTS
enum class LightState { RED, YELLOW, GREEN };

const std::string DIRECTIONS[] = {"North", "South", "East", "West"};
const std::string SHORT_DIRS[] = {"N", "S", "E", "W"};

// UTILITY FUNCTIONS
std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_ptr = std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%H:%M:%S");
    return oss.str();
}

std::string lightStateToString(LightState state) {
    switch (state) {
        case LightState::GREEN: return "GREEN";
        case LightState::YELLOW: return "YELLOW";
        case LightState::RED: return "RED";
        default: return "UNKNOWN";
    }
}

void clearScreen() {
    system("cls");
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// LOGGER CLASS TODO
// Implement logging for all traffic light state changes
class Logger {
private:
    std::string logFilePath;
    Logger() : logFilePath("data/traffic_log.txt") {}
    
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }
    
    // TODO: Implement logging to file
    // Format: timestamp,intersection,direction,state,mode
    // Example: 10:05:32,District 7,North,GREEN,AUTO
    void log(const std::string& intersection, const std::string& direction,
             const std::string& newState, bool isManual = false) {
        // TODO: Log to traffic_log.txt
    }
    
    // TODO: Read all logs from file
    std::vector<std::string> getAllLogs() {
        // TODO: Read logs from file and return as vector
        return {};
    }
    
    // TODO: Display logs in formatted table
    void displayLogs() {
        // TODO: Display all logs with headers
    }
};

// TRAFFIC LIGHT CLASS
// Demonstrates: ENCAPSULATION - private state with public interface
class TrafficLight {
private:
    LightState state;
    std::string direction;  // "North", "South", "East", "West"
    
public:
    TrafficLight() : state(LightState::RED), direction("") {}
    
    TrafficLight(const std::string& dir) : state(LightState::RED), direction(dir) {}
    
    // Getters
    LightState getState() const { return state; }
    std::string getDirection() const { return direction; }
    
    std::string getStateString() const {
        return lightStateToString(state);
    }
    
    // Setter
    void setState(LightState newState) {
        state = newState;
    }
    
    // Display helper
    std::string getDisplayChar() const {
        switch (state) {
            case LightState::GREEN: return "G";
            case LightState::YELLOW: return "Y";
            case LightState::RED: return "R";
            default: return "?";
        }
    }
};

// INTERSECTION CLASS
// Demonstrates: COMPOSITION - contains 4 TrafficLight objects
class Intersection {
private:
    std::string id;
    std::string name;
    TrafficLight lights[4];  // 0=North, 1=South, 2=East, 3=West
    
    int greenDuration;    // seconds
    int yellowDuration;   // seconds
    int redDuration;      // seconds (calculated from other directions)
    
    int currentGreenIndex;  // Which direction (0-3) is currently green
    int countdown;          // Seconds until next state change
    bool autoMode;          // true = automatic, false = manual
    
    LightState currentPhase;  // Current phase of the green light
    
    std::string assignedOperatorId;
    
public:
    Intersection() 
        : id(""), name(""), greenDuration(30), yellowDuration(5), redDuration(25),
          currentGreenIndex(0), countdown(30), autoMode(true), 
          currentPhase(LightState::GREEN), assignedOperatorId("") {
        // Initialize lights with directions
        lights[0] = TrafficLight("North");
        lights[1] = TrafficLight("South");
        lights[2] = TrafficLight("East");
        lights[3] = TrafficLight("West");
        
        // Set initial state: North is green, others red
        setGreenDirection(0);
    }
    
    Intersection(const std::string& id, const std::string& name,
                 int green = 30, int yellow = 5, int red = 25)
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
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    bool isAutoMode() const { return autoMode; }
    int getCountdown() const { return countdown; }
    std::string getAssignedOperator() const { return assignedOperatorId; }
    int getGreenDuration() const { return greenDuration; }
    int getYellowDuration() const { return yellowDuration; }
    int getRedDuration() const { return redDuration; }
    
    // Setters (for Admin configuration - Member 1)
    void setId(const std::string& newId) { id = newId; }
    void setName(const std::string& newName) { name = newName; }
    void setDurations(int green, int yellow, int red) {
        greenDuration = green;
        yellowDuration = yellow;
        redDuration = red;
    }
    void assignOperator(const std::string& opId) { assignedOperatorId = opId; }
    
    // CORE LOGIC: Set green direction
    // Ensures only ONE light is GREEN at a time
    void setGreenDirection(int newIndex) {
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
    void tick() {
        if (!autoMode) return;  // Don't tick in manual mode
        
        countdown--;
        
        if (countdown <= 0) {
            // Time to change state
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
    void toggleMode() {
        autoMode = !autoMode;
        std::cout << "Mode switched to: " << (autoMode ? "AUTO" : "MANUAL") << "\n";
    }
    
    void setAutoMode(bool mode) {
        autoMode = mode;
    }
    
    // MANUAL OVERRIDE
    // Allows operator to set any direction to GREEN
    void manualOverride(int direction) {
        if (autoMode) {
            std::cout << "Cannot override in AUTO mode. Switch to MANUAL first.\n";
            return;
        }
        
        if (direction < 0 || direction > 3) {
            std::cout << "Invalid direction. Use 0-3 (N, S, E, W).\n";
            return;
        }
        
        // First set current green to yellow briefly, then red
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
    void displayStatus() const {
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
    
    std::string getCompactStatus() const {
        std::ostringstream oss;
        oss << "N:" << lights[0].getDisplayChar() 
            << " S:" << lights[1].getDisplayChar()
            << " E:" << lights[2].getDisplayChar()
            << " W:" << lights[3].getDisplayChar();
        return oss.str();
    }
};

// USER BASE CLASS (Shared - Abstract)
// Demonstrates: ABSTRACTION with pure virtual methods
class User {
protected:
    std::string id;
    std::string username;
    std::string password;
    
public:
    User() : id(""), username(""), password("") {}
    User(const std::string& id, const std::string& uname, const std::string& pwd)
        : id(id), username(uname), password(pwd) {}
    
    virtual ~User() = default;
    
    // Pure virtual methods - MUST be implemented by derived classes
    virtual std::string getRole() const = 0;
    virtual void displayMenu() = 0;
    
    // Common methods
    std::string getId() const { return id; }
    std::string getUsername() const { return username; }
    
    bool authenticate(const std::string& pwd) const {
        return password == pwd;
    }
    
    void setId(const std::string& newId) { id = newId; }
    void setUsername(const std::string& uname) { username = uname; }
    void setPassword(const std::string& pwd) { password = pwd; }
};

// OPERATOR CLASS
// Demonstrates: INHERITANCE from User, POLYMORPHISM via virtual methods
class Operator : public User {
private:
    std::vector<std::string> assignedIntersectionIds;
    
public:
    Operator() : User() {}
    Operator(const std::string& id, const std::string& uname, const std::string& pwd)
        : User(id, uname, pwd) {}
    
    // Polymorphic methods
    std::string getRole() const override { return "OPERATOR"; }
    
    void displayMenu() override {
        std::cout << "\n=== Operator Menu ===\n";
        std::cout << "1. View Assigned Intersections\n";
        std::cout << "2. Monitor Intersection (Live View)\n";
        std::cout << "3. Switch Mode (AUTO/MANUAL)\n";
        std::cout << "4. Manual Override\n";
        std::cout << "5. Logout\n";
        std::cout << "Choice: ";
    }
    
    // Operator-specific methods
    void addAssignedIntersection(const std::string& intId) {
        assignedIntersectionIds.push_back(intId);
    }
    
    bool isAssignedTo(const std::string& intId) const {
        for (const auto& id : assignedIntersectionIds) {
            if (id == intId) return true;
        }
        return false;
    }
    
    std::vector<std::string> getAssignedIntersections() const {
        return assignedIntersectionIds;
    }
    
    void viewAssignedIntersections(const std::vector<Intersection>& allIntersections) {
        std::cout << "\n=== Your Assigned Intersections ===\n";
        
        bool found = false;
        for (const auto& intersection : allIntersections) {
            if (isAssignedTo(intersection.getId())) {
                std::cout << "- " << intersection.getName() 
                          << " (" << intersection.getId() << ")"
                          << " | " << intersection.getCompactStatus()
                          << " | Mode: " << (intersection.isAutoMode() ? "AUTO" : "MANUAL")
                          << "\n";
                found = true;
            }
        }
        
        if (!found) {
            std::cout << "No intersections assigned to you.\n";
        }
    }
    
    // Live monitoring with auto-refresh
    void monitorIntersection(Intersection& intersection) {
        std::cout << "\n=== Live Monitoring: " << intersection.getName() << " ===\n";
        std::cout << "(Press Ctrl+C to stop, or wait for timeout)\n\n";
        
        for (int i = 0; i < 30; i++) {  // Monitor for 30 seconds
            clearScreen();
            std::cout << "=== Live Monitoring: " << intersection.getName() << " ===\n";
            std::cout << "Time: " << getCurrentTimestamp() << "\n";
            
            intersection.displayStatus();
            
            // Tick the intersection (advance time)
            intersection.tick();
            
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        
        std::cout << "\nMonitoring ended.\n";
    }
    
    void switchMode(Intersection& intersection) {
        intersection.toggleMode();
    }
    
    void performOverride(Intersection& intersection) {
        if (intersection.isAutoMode()) {
            std::cout << "Switch to MANUAL mode first!\n";
            return;
        }
        
        std::cout << "\nSelect direction to set GREEN:\n";
        std::cout << "0. North\n";
        std::cout << "1. South\n";
        std::cout << "2. East\n";
        std::cout << "3. West\n";
        std::cout << "Choice: ";
        
        int direction;
        std::cin >> direction;
        
        intersection.manualOverride(direction);
    }
};

// ADMIN CLASS TODO
// Implement all admin functionality
class Admin : public User {
public:
    Admin() : User() {}
    Admin(const std::string& id, const std::string& uname, const std::string& pwd)
        : User(id, uname, pwd) {}
    
    std::string getRole() const override { return "ADMIN"; }
    
    void displayMenu() override {
        std::cout << "\n=== Admin Menu ===\n";
        std::cout << "1. Add Intersection\n";
        std::cout << "2. Remove Intersection\n";
        std::cout << "3. Configure Intersection Timing\n";
        std::cout << "4. Assign Operator to Intersection\n";
        std::cout << "5. View All Activity Logs\n";
        std::cout << "6. List All Intersections\n";
        std::cout << "7. Logout\n";
        std::cout << "Choice: ";
    }
    
    // TODO: Add new intersection
    // Prompt for ID, name, green/yellow/red durations
    void addIntersection(std::vector<Intersection>& intersections) {
        // TODO: Implement add intersection
    }
    
    // TODO: Remove intersection by ID
    void removeIntersection(std::vector<Intersection>& intersections) {
        // TODO: Implement remove intersection
    }
    
    // TODO: Configure timing for an intersection
    void configureIntersection(std::vector<Intersection>& intersections) {
        // TODO: Implement configure timing
    }
    
    // TODO: Assign operator to intersection
    void assignOperatorToIntersection(std::vector<Intersection>& intersections,
                                      std::vector<Operator>& operators) {
        // TODO: Implement assign operator
    }
    
    // TODO: View all activity logs
    void viewAllLogs() {
        // TODO: Call Logger::getInstance().displayLogs()
    }
    
    // TODO: List all intersections
    void listIntersections(const std::vector<Intersection>& intersections) {
        // TODO: Display all intersections with their settings
    }
};

// FILE MANAGER TODO
// Implement save/load for data persistence
class FileManager {
private:
    std::string dataDir;
    
public:
    FileManager(const std::string& dir = "data/") : dataDir(dir) {
        system(("mkdir " + dataDir + " 2>nul").c_str());
    }
    
    // TODO: Save intersections to CSV
    // Format: id,name,green,yellow,red,operator
    void saveIntersections(const std::vector<Intersection>& intersections) {
        // TODO: Save to data/intersections.csv
    }
    
    // TODO: Load intersections from CSV
    std::vector<Intersection> loadIntersections() {
        // TODO: Load from data/intersections.csv
        return {};
    }
    
    // TODO: Save users to CSV
    void saveUsers(const std::vector<Admin>& admins, const std::vector<Operator>& operators) {
        // TODO: Save to data/users.csv
    }
    
    // TODO: Load users from CSV
    void loadUsers(std::vector<Admin>& admins, std::vector<Operator>& operators) {
        // TODO: Load from data/users.csv
    }
};

// MAIN PROGRAM
int main() {
    // Initialize file manager and load data
    FileManager fileManager;
    std::vector<Intersection> intersections = fileManager.loadIntersections();
    
    // Create sample users (Member 1 should load from file)
    std::vector<Admin> admins;
    std::vector<Operator> operators;
    
    // Default admin and operator for testing
    admins.emplace_back("ADM001", "admin", "admin123");
    operators.emplace_back("OP001", "operator1", "op123");
    
    // If no intersections exist, create a sample one
    if (intersections.empty()) {
        intersections.emplace_back("INT001", "Nguyen Van Linh", 30, 5, 25);
        intersections.emplace_back("INT002", "District 7 Junction", 25, 5, 20);
        operators[0].addAssignedIntersection("INT001");
        operators[0].addAssignedIntersection("INT002");
    }
    
    // Main program loop
    bool running = true;
    
    while (running) {
        clearScreen();
        std::cout << "========================================\n";
        std::cout << "   TRAFFIC LIGHT MANAGEMENT SYSTEM\n";
        std::cout << "        Smart City Systems\n";
        std::cout << "========================================\n\n";
        
        std::cout << "1. Login as Admin\n";
        std::cout << "2. Login as Operator\n";
        std::cout << "3. Exit\n";
        std::cout << "\nChoice: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 3) {
            // Save data before exit
            fileManager.saveIntersections(intersections);
            std::cout << "\nGoodbye!\n";
            running = false;
            break;
        }
        
        std::string username, password;
        std::cout << "\nUsername: ";
        std::cin >> username;
        std::cout << "Password: ";
        std::cin >> password;
        
        if (choice == 1) {
            // Admin login
            bool authenticated = false;
            Admin* currentAdmin = nullptr;
            
            for (auto& admin : admins) {
                if (admin.getUsername() == username && admin.authenticate(password)) {
                    authenticated = true;
                    currentAdmin = &admin;
                    break;
                }
            }
            
            if (!authenticated) {
                std::cout << "Invalid credentials.\n";
                pauseScreen();
                continue;
            }
            
            // Admin menu loop
            bool adminLoggedIn = true;
            while (adminLoggedIn) {
                clearScreen();
                std::cout << "Welcome, " << currentAdmin->getUsername() << " (Admin)\n";
                currentAdmin->displayMenu();
                
                int adminChoice;
                std::cin >> adminChoice;
                
                switch (adminChoice) {
                    case 1:
                        currentAdmin->addIntersection(intersections);
                        pauseScreen();
                        break;
                    case 2:
                        currentAdmin->removeIntersection(intersections);
                        pauseScreen();
                        break;
                    case 3:
                        currentAdmin->configureIntersection(intersections);
                        pauseScreen();
                        break;
                    case 4:
                        currentAdmin->assignOperatorToIntersection(intersections, operators);
                        pauseScreen();
                        break;
                    case 5:
                        currentAdmin->viewAllLogs();
                        pauseScreen();
                        break;
                    case 6:
                        currentAdmin->listIntersections(intersections);
                        pauseScreen();
                        break;
                    case 7:
                        adminLoggedIn = false;
                        break;
                    default:
                        std::cout << "Invalid choice.\n";
                        pauseScreen();
                }
            }
        }
        else if (choice == 2) {
            // Operator login
            bool authenticated = false;
            Operator* currentOperator = nullptr;
            
            for (auto& op : operators) {
                if (op.getUsername() == username && op.authenticate(password)) {
                    authenticated = true;
                    currentOperator = &op;
                    break;
                }
            }
            
            if (!authenticated) {
                std::cout << "Invalid credentials.\n";
                pauseScreen();
                continue;
            }
            
            // Operator menu loop
            bool operatorLoggedIn = true;
            while (operatorLoggedIn) {
                clearScreen();
                std::cout << "Welcome, " << currentOperator->getUsername() << " (Operator)\n";
                currentOperator->displayMenu();
                
                int opChoice;
                std::cin >> opChoice;
                
                switch (opChoice) {
                    case 1:
                        currentOperator->viewAssignedIntersections(intersections);
                        pauseScreen();
                        break;
                    case 2: {
                        // Select intersection to monitor
                        std::cout << "\nEnter Intersection ID to monitor: ";
                        std::string intId;
                        std::cin >> intId;
                        
                        bool found = false;
                        for (auto& intersection : intersections) {
                            if (intersection.getId() == intId && 
                                currentOperator->isAssignedTo(intId)) {
                                currentOperator->monitorIntersection(intersection);
                                found = true;
                                break;
                            }
                        }
                        if (!found) {
                            std::cout << "Intersection not found or not assigned to you.\n";
                            pauseScreen();
                        }
                        break;
                    }
                    case 3: {
                        std::cout << "\nEnter Intersection ID: ";
                        std::string intId;
                        std::cin >> intId;
                        
                        for (auto& intersection : intersections) {
                            if (intersection.getId() == intId && 
                                currentOperator->isAssignedTo(intId)) {
                                currentOperator->switchMode(intersection);
                                break;
                            }
                        }
                        pauseScreen();
                        break;
                    }
                    case 4: {
                        std::cout << "\nEnter Intersection ID: ";
                        std::string intId;
                        std::cin >> intId;
                        
                        for (auto& intersection : intersections) {
                            if (intersection.getId() == intId && 
                                currentOperator->isAssignedTo(intId)) {
                                currentOperator->performOverride(intersection);
                                break;
                            }
                        }
                        pauseScreen();
                        break;
                    }
                    case 5:
                        operatorLoggedIn = false;
                        break;
                    default:
                        std::cout << "Invalid choice.\n";
                        pauseScreen();
                }
            }
        }
    }
    
    return 0;
} 
