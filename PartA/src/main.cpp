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
    // Initialize file manager and load data
    FileManager fileManager;
    std::vector<Intersection> intersections = fileManager.loadIntersections();
    
    // Create sample users (Member 1 should load from file)
    std::vector<Admin> admins;
    std::vector<Operator> operators;
    
    // Default admin and operator for testing
    admins.emplace_back("ADM001", "admin", "admin123");
    operators.emplace_back("OP001", "operator1", "op123");
    
    // If no intersections exist, create sample ones
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
