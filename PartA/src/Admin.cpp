/*
 * Admin.cpp - Admin class implementation
 * Part A: Traffic Light Management System
 */

#include "Admin.h"
#include "Operator.h"
#include "Logger.h"

Admin::Admin() : User() {}

Admin::Admin(const std::string& id, const std::string& uname, const std::string& pwd)
    : User(id, uname, pwd) {}

std::string Admin::getRole() const { 
    return "ADMIN"; 
}

void Admin::displayMenu() {
    std::cout << "\n============ ADMIN MENU ============\n";
    std::cout << "1. View All Intersections\n";
    std::cout << "2. View Activity Logs\n";
    std::cout << "3. Logout\n";
    std::cout << "====================================\n";
    std::cout << "Choice: ";
}

void Admin::manageIntersections(std::vector<Intersection>& intersections,
                                 std::vector<Operator>& operators) {
    bool managing = true;
    
    while (managing) {
        clearScreen();
        std::cout << "================= ALL INTERSECTIONS =================\n";
        
        if (intersections.empty()) {
            std::cout << "\nNo intersections configured.\n";
        } else {
            std::cout << std::setw(8) << "ID" << " | "
                      << std::setw(22) << "Name" << " | "
                      << std::setw(4) << "G" << " | "
                      << std::setw(4) << "Y" << " | "
                      << std::setw(4) << "R" << " | "
                      << "Operator\n";
            std::cout << std::string(68, '-') << "\n";
            
            for (const auto& i : intersections) {
                std::cout << std::setw(8) << i.getId() << " | "
                          << std::setw(22) << i.getName() << " | "
                          << std::setw(4) << i.getGreenDuration() << " | "
                          << std::setw(4) << i.getYellowDuration() << " | "
                          << std::setw(4) << i.getRedDuration() << " | "
                          << (i.getAssignedOperator().empty() ? "None" : i.getAssignedOperator())
                          << "\n";
            }
            std::cout << std::string(68, '=') << "\n";
        }
        
        std::cout << "\n[A] Add Intersection  [Q] Back to Menu\n";
        std::cout << "Or enter Intersection ID to edit/remove: ";
        
        std::string input;
        std::cin >> input;
        
        if (input == "q" || input == "Q") {
            managing = false;
            continue;
        }
        
        if (input == "a" || input == "A") {
            // Add new intersection
            addIntersection(intersections);
            pauseScreen();
            continue;
        }
        
        // Try to find intersection by ID
        Intersection* target = nullptr;
        for (auto& intersection : intersections) {
            if (intersection.getId() == input) {
                target = &intersection;
                break;
            }
        }
        
        if (!target) {
            std::cout << "Intersection '" << input << "' not found.\n";
            pauseScreen();
            continue;
        }
        
        // Show intersection details and options
        std::cout << "\n--- " << target->getName() << " (" << target->getId() << ") ---\n";
        std::cout << "Timings: Green=" << target->getGreenDuration() 
                  << "s, Yellow=" << target->getYellowDuration() 
                  << "s, Red=" << target->getRedDuration() << "s\n";
        std::cout << "Operator: " << (target->getAssignedOperator().empty() ? "None" : target->getAssignedOperator()) << "\n";
        
        std::cout << "\n1. Edit Timings\n";
        if (target->getAssignedOperator().empty()) {
            std::cout << "2. Assign Operator\n";
        } else {
            std::cout << "2. Remove Operator\n";
        }
        std::cout << "3. Delete Intersection\n";
        std::cout << "4. Cancel\n";
        std::cout << "Choice: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                // Edit timings
                int green, yellow, red;
                std::cout << "\nEnter new Green duration (seconds): ";
                std::cin >> green;
                std::cout << "Enter new Yellow duration (seconds): ";
                std::cin >> yellow;
                std::cout << "Enter new Red duration (seconds): ";
                std::cin >> red;
                
                if (green > 0 && yellow > 0 && red > 0) {
                    target->setDurations(green, yellow, red);
                    std::cout << "Success! Timings updated (G=" << green << "s, Y=" << yellow << "s, R=" << red << "s).\n";
                } else {
                    std::cout << "Error: Light durations must be positive.\n";
                }
                pauseScreen();
                break;
            }
            case 2: {
                if (target->getAssignedOperator().empty()) {
                    // Assign operator
                    if (operators.empty()) {
                        std::cout << "No operators available.\n";
                    } else {
                        std::cout << "\nAvailable Operators:\n";
                        for (const auto& op : operators) {
                            std::cout << "  " << op.getId() << " - " << op.getUsername() << "\n";
                        }
                        
                        std::string opId;
                        std::cout << "\nEnter Operator ID (or 'q' to cancel): ";
                        std::cin >> opId;
                        
                        if (opId == "q" || opId == "Q") {
                            std::cout << "Cancelled.\n";
                            break;
                        }
                        
                        Operator* targetOp = nullptr;
                        for (auto& op : operators) {
                            if (op.getId() == opId) {
                                targetOp = &op;
                                break;
                            }
                        }
                        
                        if (targetOp) {
                            target->assignOperator(opId);
                            targetOp->addAssignedIntersection(target->getId());
                            std::cout << "Success! Operator assigned.\n";
                        } else {
                            std::cout << "Operator not found.\n";
                        }
                    }
                } else {
                    // Remove operator
                    target->assignOperator("");
                    std::cout << "Operator removed.\n";
                }
                pauseScreen();
                break;
            }
            case 3: {
                // Delete intersection
                std::cout << "\nAre you sure you want to delete '" << target->getName() << "'? (y/n): ";
                char confirm;
                std::cin >> confirm;
                
                if (confirm == 'y' || confirm == 'Y') {
                    std::string name = target->getName();
                    for (auto it = intersections.begin(); it != intersections.end(); ++it) {
                        if (it->getId() == target->getId()) {
                            intersections.erase(it);
                            break;
                        }
                    }
                    std::cout << "Deleted '" << name << "'.\n";
                }
                pauseScreen();
                break;
            }
            case 4:
            default:
                break;
        }
    }
}

void Admin::addIntersection(std::vector<Intersection>& intersections) {
    std::string name;
    int green, yellow, red;
    
    std::cout << "\n===== ADD NEW INTERSECTION =====\n";
    // Auto-generate intersection ID
    int maxIdNum = 0;
    for (const auto& i : intersections) {
        std::string existingId = i.getId();
        if (existingId.length() > 3 && existingId.substr(0, 3) == "INT") {
            try {
                int num = std::stoi(existingId.substr(3));
                if (num > maxIdNum) maxIdNum = num;
            } catch (...) {
                // Skip non-numeric IDs
            }
        }
    }
    std::string id = "INT" + std::string(3 - std::to_string(maxIdNum + 1).length(), '0') + std::to_string(maxIdNum + 1);
    
    std::cin.ignore();
    std::cout << "Enter Name (or 'q' to cancel): ";
    std::getline(std::cin, name);
    if (name == "q" || name == "Q") {
        std::cout << "Cancelled.\n";
        return;
    }
    
    std::cout << "Green duration (seconds)\n";
    std::cout << "Enter (0) for default or (-1) to cancel:";
    std::cin >> green;
    if (green == -1) {
        std::cout << "Cancelled.\n";
        return;
    }
    if (green <= 0) green = 30;
    
    std::cout << "Yellow duration (seconds)\n";
    std::cout << "Enter (0) for default or (-1) to cancel:";
    std::cin >> yellow;
    if (yellow == -1) {
        std::cout << "Cancelled.\n";
        return;
    }
    if (yellow <= 0) yellow = 5;
    
    std::cout << "Red duration (seconds)\n";
    std::cout << "Enter (0) for default or (-1) to cancel:";
    std::cin >> red;
    if (red == -1) {
        std::cout << "Cancelled.\n";
        return;
    }
    if (red <= 0) red = 35;
    
    intersections.emplace_back(id, name, green, yellow, red);
    std::cout << "\nSuccessfully added intersection!\n";
    std::cout << "  Name: " << name << "\n";
    std::cout << "  ID: " << id << "\n";
    std::cout << "  Timings: Green=" << green << "s, Yellow=" << yellow << "s, Red=" << red << "s\n";
}

void Admin::viewAllLogs() {
    Logger::getInstance().displayLogs();
}

