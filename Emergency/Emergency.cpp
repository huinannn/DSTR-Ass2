#include "Emergency.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
using namespace std;

// Constructor
EmergencyManager::EmergencyManager() {
    size = 0;
    nextID = 1;
    loadFromCSV();
}

// Helpers
bool EmergencyManager::isFull() const {
    return size >= MAX_EMERGENCY;
}

bool EmergencyManager::isEmpty() const {
    return size == 0;
}

// Safe Input
string EmptyVal(const string &prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!input.empty()) 
            return input;

        cout << "Input cannot be empty!" << endl;
    }
}

int PriorityVal(const string &prompt) {
    while (true) {
        string raw = EmptyVal(prompt);

        bool isDigits = true;
        for (char c : raw) {
            if (!isdigit(c)) { 
                isDigits = false; 
                break; 
            }
        }

        int val = isDigits ? stoi(raw) : -1;

        if (val >= 1 && val <= 10) 
            return val;

        cout << "Please enter a number between 1-10 only!" << endl;
    }
}

int MenuChoiceVal(const string &prompt, int min, int max) {
    while (true) {
        string raw = EmptyVal(prompt);

        bool isDigits = true;
        for (char c : raw) {
            if (!isdigit(c)) { 
                isDigits = false; 
                break; 
            }
        }

        int val = isDigits ? stoi(raw) : -1;

        if (val >= min && val <= max) 
            return val;

        cout << "Please enter a number between " << min << " and " << max << "!" << endl;
    }
}

// Load CSV
void EmergencyManager::loadFromCSV() {
    ifstream file(EMERGENCY_CSV);
    if (!file.is_open()) 
        return;

    string line;
    size = 0;

    getline(file, line);

    while (getline(file, line)) {
        if (line.empty()) 
            continue;

        stringstream ss(line);
        string idStr, name, type, priStr;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, type, ',');
        getline(ss, priStr, ',');

        idStr.erase(0, idStr.find_first_not_of(" "));
        name.erase(0, name.find_first_not_of(" "));
        type.erase(0, type.find_first_not_of(" "));
        priStr.erase(0, priStr.find_first_not_of(" "));

        Emergency e;
        e.id = idStr;
        e.name = name;
        e.type = type;
        e.priority = stoi(priStr);

        cases[size++] = e;

        if (idStr[0] == 'P') {
            int num = stoi(idStr.substr(1));
            if (num >= nextID) 
                nextID = num + 1;
        }

        if (size >= MAX_EMERGENCY) 
            break;
    }

    file.close();
}

// Save CSV
void EmergencyManager::saveToCSV() const {
    ofstream file(EMERGENCY_CSV);

    file << "ID, Patient Name, Emergency Type, Priority Level" << endl;

    for (int i = 0; i < size; i++) {
        file << cases[i].id << ", "
             << cases[i].name << ", "
             << cases[i].type << ", "
             << cases[i].priority << endl;
    }

    file.close();
}

// Log Case
void EmergencyManager::logCase() {
    if (isFull()) {
        cout << "Emergency list is full!" << endl << endl;
        return;
    }

    cout << endl << "============= Log Emergency Case ==============" << endl;

    Emergency e;

    char buffer[10];
    sprintf(buffer, "P%03d", nextID++);
    e.id = buffer;

    e.name = EmptyVal("Enter patient name: ");
    e.type = EmptyVal("Enter emergency type: ");
    e.priority = PriorityVal("Priority (1 = critical, 10 = mild): ");

    cases[size++] = e;

    saveToCSV();
    cout << "Emergency case added!" << endl << endl;
}

// Process Critical Case
void EmergencyManager::processCritical() {
    if (isEmpty()) {
        cout << endl << "No emergency cases available!" << endl;
        return;
    }

    int bestIndex = 0;
    for (int i = 1; i < size; i++) {
        if (cases[i].priority < cases[bestIndex].priority)
            bestIndex = i;
    }

    Emergency c = cases[bestIndex];

    cout << endl << "============== Most Critical Case ==============" << endl;
    cout << "ID       : " << c.id << endl;
    cout << "Name     : " << c.name << endl;
    cout << "Type     : " << c.type << endl;
    cout << "Priority : " << c.priority << endl;
    cout << "===============================================" << endl;

    string confirm;
    while (true) {
        confirm = EmptyVal("Do you want to process this case? (Y/N): ");

        for (char &ch : confirm)
            ch = toupper(ch);

        if (confirm == "Y") {
            break;
        }
        else if (confirm == "N") {
            cout << "Cancelled! Returning to menu..." << endl << endl;
            return;
        }
        else {
            cout << "Please enter Y or N only!" << endl;
        }
    }

    for (int i = bestIndex; i < size - 1; i++) {
        cases[i] = cases[i + 1];
    }

    size--;
    saveToCSV();

    cout << "Case processed and removed!" << endl << endl;
}

// View Cases
void EmergencyManager::viewCases() const {
    if (isEmpty()) {
        cout << endl << "No cases available!" << endl << endl;
        return;
    }

    cout << endl << "================ Pending Emergency Cases ===============" << endl;

    Emergency sorted[MAX_EMERGENCY];
    for (int i = 0; i < size; i++) sorted[i] = cases[i];

    sort(sorted, sorted + size,
         [](const Emergency &a, const Emergency &b) {
             return a.priority < b.priority;
         });

    cout << left << setw(10) << "ID" << setw(16) << "Name" << setw(18) << "Type" << setw(10) << "Priority" << endl;

    cout << string(56, '-') << endl;

    for (int i = 0; i < size; i++) {
        cout << setw(8) << sorted[i].id << setw(15) << sorted[i].name << setw(24) << sorted[i].type << sorted[i].priority << endl;
    }

    cout << string(56, '-') << endl << endl;
}

// Menu
void emergencyMenu(EmergencyManager &manager) {
    int choice;

    do {
        cout << "======== Emergency Department Officer =========" << endl;
        cout << "1. Log Emergency Case" << endl;
        cout << "2. Process Most Critical Case" << endl;
        cout << "3. View Pending Emergency Cases" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "===============================================" << endl;

        choice = MenuChoiceVal("Enter your choice: ", 1, 4);

        if (choice == 1) 
            manager.logCase();
        else if (choice == 2) 
            manager.processCritical();
        else if (choice == 3) 
            manager.viewCases();
        else if (choice == 4) 
            cout << "Exiting Emergency Department Officer. Goodbye!" << endl;
            cout << "Returning to main menu..." << endl;

    } while (choice != 4);
}

int main() {
    EmergencyManager manager;
    emergencyMenu(manager);
    return 0;
}