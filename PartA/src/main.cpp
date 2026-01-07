/*
 * main.cpp - Program Entry Point
 * Part A: Traffic Light Management System
 * Smart City Systems - Group Project
 * 
 * OOP Principles Demonstrated:
 * - Encapsulation: Private members with public interfaces
 * - Abstraction: User base class with pure virtual methods
 * - Inheritance: Admin and Operator inherit from User
 * - Polymorphism: Virtual displayMenu() for different user types
 */

#include "Admin.h"
#include "Operator.h"
#include "FileManager.h"

int main() {
    std::cout << "========================================\n";
    std::cout << "   TRAFFIC LIGHT MANAGEMENT SYSTEM\n";
    std::cout << "        Smart City Systems\n";
    std::cout << "========================================\n\n";
    
    // Initialize file manager and load data
    FileManager fileManager;
    std::vector<Intersection> intersections = fileManager.loadIntersections();
    
    // Create user vectors
    std::vector<Admin> admins;
    std::vector<Operator> operators;
    
    // Try to load users from file
    fileManager.loadUsers(admins, operators);
    
    // If no users loaded, create defaults
    if (admins.empty()) {
        admins.emplace_back("ADM001", "admin", "admin123");
        std::cout << "Created default admin (username: admin, password: admin123)\n";
    }
    if (operators.empty()) {
        operators.emplace_back("OP001", "operator1", "op123");
        std::cout << "Created default operator (username: operator1, password: op123)\n";
    }
    
    // If no intersections exist, create samples
    if (intersections.empty()) {
        intersections.emplace_back("INT001", "Nguyen Van Linh", 30, 5, 25);
        intersections.emplace_back("INT002", "District 7 Junction", 25, 5, 20);
        operators[0].addAssignedIntersection("INT001");
        operators[0].addAssignedIntersection("INT002");
        std::cout << "Created 2 sample intersections.\n";
    }
    
    std::cout << "\n";
    
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
        std::cout << "3. Create New Account\n";
        std::cout << "4. Exit\n";
        std::cout << "\nChoice: ";
        
        int choice;
        std::cin >> choice;
        
        if (choice == 4) {
            // Save all data before exit
            std::cout << "\nSaving data...\n";
            fileManager.saveIntersections(intersections);
            fileManager.saveUsers(admins, operators);
            std::cout << "\nGoodbye!\n";
            running = false;
            break;
        }
        
        if (choice == 3) {
            // Create new account
            std::cout << "\n===== CREATE NEW ACCOUNT =====\n";
            std::cout << "1. Admin Account\n";
            std::cout << "2. Operator Account\n";
            std::cout << "3. Cancel\n";
            std::cout << "Choice: ";
            
            int accountType;
            std::cin >> accountType;
            
            if (accountType == 3) continue;
            
            std::string id, username, password;
            
            if (accountType == 1) {
                // Auto-generate Admin ID
                int nextId = admins.size() + 1;
                id = "ADM" + std::string(3 - std::to_string(nextId).length(), '0') + std::to_string(nextId);
                
                std::cout << "\nEnter Username: ";
                std::cin >> username;
                std::cout << "Enter Password: ";
                std::cin >> password;
                
                admins.emplace_back(id, username, password);
                std::cout << "\nAdmin account created!\n";
                std::cout << "Your ID: " << id << "\n";
                std::cout << "Username: " << username << "\n";
                pauseScreen();
            }
            else if (accountType == 2) {
                // Auto-generate Operator ID
                int nextId = operators.size() + 1;
                id = "OP" + std::string(3 - std::to_string(nextId).length(), '0') + std::to_string(nextId);
                
                std::cout << "\nEnter Username: ";
                std::cin >> username;
                std::cout << "Enter Password: ";
                std::cin >> password;
                
                operators.emplace_back(id, username, password);
                std::cout << "\nOperator account created!\n";
                std::cout << "Your ID: " << id << "\n";
                std::cout << "Username: " << username << "\n";
                pauseScreen();
            }
            continue;
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
                        currentAdmin->manageIntersections(intersections, operators);
                        break;
                    case 2:
                        currentAdmin->viewAllLogs();
                        break;
                    case 3:
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
                    case 1: {
                        // Show available intersections first
                        std::cout << "\n=== Your Assigned Intersections ===\n";
                        for (const auto& intersection : intersections) {
                            if (currentOperator->isAssignedTo(intersection.getId())) {
                                std::cout << "  " << intersection.getId() << " - " << intersection.getName() << "\n";
                            }
                        }
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
                    case 2:
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
