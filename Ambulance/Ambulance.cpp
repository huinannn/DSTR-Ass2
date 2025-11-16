#include "Ambulance.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <algorithm>   // for all_of, sort
#include <vector>
#include <cctype>      // for isdigit
#include <stdexcept>   // for stoi exception handling
#include <iomanip>     // for setw

using namespace std;

// ==============================================
//   AmbulanceQueue methods
// ==============================================

AmbulanceQueue::AmbulanceQueue() {
    front = 0;
    rear  = -1;
    count = 0;
}

bool AmbulanceQueue::isEmpty() const {
    return count == 0;
}

bool AmbulanceQueue::isFull() const {
    return count == MAX_AMBULANCES;
}

bool AmbulanceQueue::enqueue(const Ambulance& a) {
    if (isFull()) {
        cerr << "[ERROR] Queue is full. Cannot enqueue.\n";
        return false;
    }
    rear = (rear + 1) % MAX_AMBULANCES;
    arr[rear] = a;
    count++;
    return true;
}

bool AmbulanceQueue::dequeue(Ambulance& removed) {
    if (isEmpty()) {
        cerr << "[ERROR] Queue is empty. Cannot dequeue.\n";
        return false;
    }
    removed = arr[front];
    front = (front + 1) % MAX_AMBULANCES;
    count--;
    return true;
}

bool AmbulanceQueue::rotate() {
    if (count <= 1) return false;

    Ambulance first;
    if (!dequeue(first)) return false;
    if (!enqueue(first)) return false;

    return true;
}

// ==============================================
//  Display queue in table format
// ==============================================
void AmbulanceQueue::display() const {
    if (isEmpty()) {
        cout << "No ambulances in the schedule.\n";
        return;
    }

    cout << "\n=============== Current Queue ================\n";
    cout << "| " << setw(4) << "No" 
         << " | " << setw(4) << "ID" 
         << " | " << setw(12) << "Plate" 
         << " | " << setw(20) << "Driver" 
         << " | " << setw(9) << "Shift" << " |\n";
    cout << "---------------------------------------------\n";

    int index = front;
    for (int i = 0; i < count; i++) {
        const Ambulance& a = arr[index];
        string shiftName = (a.shift == 0 ? "Morning" : (a.shift == 1 ? "Afternoon" : "Midnight"));

        cout << "| " << setw(4) << (i + 1)
             << " | " << setw(4) << a.id
             << " | " << setw(12) << a.plate
             << " | " << setw(20) << a.driverName
             << " | " << setw(9) << shiftName << " |\n";

        index = (index + 1) % MAX_AMBULANCES;
    }

    cout << "=============================================\n";
}

int AmbulanceQueue::size() const {
    return count;
}

bool AmbulanceQueue::getAt(int index, Ambulance& out) const {
    if (index < 0 || index >= count) {
        cerr << "[ERROR] getAt() index out of range: " << index << "\n";
        return false;
    }
    int actualIndex = (front + index) % MAX_AMBULANCES;
    out = arr[actualIndex];
    return true;
}

void AmbulanceQueue::clear() {
    front = 0;
    rear  = -1;
    count = 0;
}

// ==============================================
//   AmbulanceManager methods
// ==============================================

AmbulanceManager::AmbulanceManager(const std::string& file)
    : fileName(file) {}

// Helper: check if ID exists in current queue
bool AmbulanceManager::idExists(int id) const {
    Ambulance temp;
    for (int i = 0; i < queue.size(); ++i) {
        if (queue.getAt(i, temp)) {
            if (temp.id == id) return true;
        }
    }
    return false;
}

// Helper: check if Plate exists
bool AmbulanceManager::plateExists(const string& plate) const {
    Ambulance temp;
    for (int i = 0; i < queue.size(); ++i) {
        if (queue.getAt(i, temp)) {
            if (plate == temp.plate) return true;
        }
    }
    return false;
}

// Helper: generate smallest positive integer ID not in use
int AmbulanceManager::generateNewID() const {
    int id = 1;
    while (idExists(id)) {
        ++id;
        if (id > 1000000) break; // safety guard
    }
    return id;
}

void AmbulanceManager::loadFromFile() {
    ifstream inFile(fileName);

    if (!inFile.is_open()) {
        cout << "[INFO] Data file not found: " << fileName << ". Starting fresh.\n";
        return;
    }

    queue.clear();
    string line;
    int lineNo = 0;

    while (getline(inFile, line)) {
        ++lineNo;
        if (line.empty()) continue;

        stringstream ss(line);
        string idStr, plateStr, driverStr, shiftStr;

        if (!getline(ss, idStr, ',')) continue;
        if (!getline(ss, plateStr, ',')) continue;
        if (!getline(ss, driverStr, ',')) {
            driverStr = "";
        }
        if (!getline(ss, shiftStr)) shiftStr = "";

        auto trim = [](string &s){
            size_t a = s.find_first_not_of(" \t\r\n");
            size_t b = s.find_last_not_of(" \t\r\n");
            if (a == string::npos) { s.clear(); return; }
            s = s.substr(a, b - a + 1);
        };
        trim(idStr); trim(plateStr); trim(driverStr); trim(shiftStr);

        if (idStr.empty() || !all_of(idStr.begin(), idStr.end(), ::isdigit)) continue;
        int id = stoi(idStr);

        Ambulance a;
        a.id = id;
        strncpy(a.plate, plateStr.c_str(), sizeof(a.plate)-1); a.plate[sizeof(a.plate)-1]='\0';
        strncpy(a.driverName, driverStr.c_str(), sizeof(a.driverName)-1); a.driverName[sizeof(a.driverName)-1]='\0';

        int shiftVal = 0;
        if (!shiftStr.empty() && all_of(shiftStr.begin(), shiftStr.end(), ::isdigit)) shiftVal = stoi(shiftStr);
        if (shiftVal < 0 || shiftVal > 2) shiftVal = 0;
        a.shift = shiftVal;

        if (!queue.enqueue(a)) break;
    }

    inFile.close();
    cout << "[INFO] Ambulance data loaded from " << fileName << ".\n";
}

void AmbulanceManager::saveToFile() const {
    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cerr << "[ERROR] Cannot write to file: " << fileName << "\n";
        return;
    }

    Ambulance temp;
    for (int i = 0; i < queue.size(); ++i) {
        if (queue.getAt(i, temp)) {
            outFile << temp.id << ","
                    << temp.plate << ","
                    << temp.driverName << ","
                    << temp.shift << "\n";
        }
    }

    outFile.close();
    cout << "[INFO] Ambulance data saved to " << fileName << ".\n";
}

void AmbulanceManager::registerAmbulance() {
    if (queue.isFull()) {
        cout << "[ERROR] Schedule is full. Cannot register more ambulances.\n";
        return;
    }

    Ambulance a;
    memset(&a, 0, sizeof(a));

    // ========== ID input with uniqueness ==========
    while (true) {
        cout << "Enter Ambulance ID (number, 0 = auto-generate): ";
        if (!(cin >> a.id)) {
            cout << "[ERROR] Please enter a valid number.\n";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (a.id == 0) {
            a.id = generateNewID();
            cout << "[INFO] Assigned new ID: " << a.id << "\n";
            break;
        }

        if (a.id < 0) {
            cout << "[ERROR] ID must be positive.\n"; continue;
        }

        if (idExists(a.id)) {
            cout << "[ERROR] ID already exists. Try another or enter 0 for auto-generate.\n"; continue;
        }
        break;
    }

    // Plate number with uniqueness check
    while (true) {
        cout << "Enter Plate Number: ";
        cin.getline(a.plate, sizeof(a.plate));
        if (strlen(a.plate) == 0) { cout << "[ERROR] Plate cannot be empty.\n"; continue; }
        if (plateExists(a.plate)) { cout << "[ERROR] Plate already exists.\n"; continue; }
        break;
    }

    // Driver
    cout << "Enter Driver Name: ";
    cin.getline(a.driverName, sizeof(a.driverName));
    while (strlen(a.driverName) == 0) { cout << "[ERROR] Name cannot be empty. Re-enter: "; cin.getline(a.driverName, sizeof(a.driverName)); }

    // ======== Shift selection ========
    cout << "\nSelect Shift:\n1. Morning (4 AM - 12 PM)\n2. Afternoon (12 PM - 8 PM)\n3. Midnight (8 PM - 4 AM)\n";

    int shiftChoice = 0;
    while (true) {
        cout << "Enter choice (1-3): ";
        if ((cin >> shiftChoice) && (shiftChoice >= 1 && shiftChoice <= 3)) break;
        cout << "[ERROR] Enter 1–3 only.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    a.shift = shiftChoice - 1;

    if (queue.enqueue(a)) cout << "Ambulance registered successfully (ID " << a.id << ").\n";
    else cerr << "[ERROR] Failed to add ambulance.\n";
}

void AmbulanceManager::rotateShift() {
    int n = queue.size();
    if (n == 0) { cout << "[ERROR] No ambulances to rotate.\n"; return; }

    vector<Ambulance> tempList; tempList.reserve(n);
    Ambulance tmp;

    for (int i = 0; i < n; ++i) {
        if (queue.dequeue(tmp)) { tmp.shift = (tmp.shift + 1) % 3; tempList.push_back(tmp); }
        else cerr << "[ERROR] Unexpected dequeue error during rotation.\n";
    }

    for (const auto& a : tempList) {
        if (!queue.enqueue(a)) cerr << "[ERROR] Failed to enqueue during rotation.\n";
    }

    cout << "[INFO] All ambulance shifts rotated (Morning->Afternoon->Midnight->Morning).\n";
}

// ==============================================
//  Display sorted schedule in table format
// ==============================================
void AmbulanceManager::displaySchedule() const {
    if (queue.isEmpty()) { cout << "No ambulances in the schedule.\n"; return; }

    vector<Ambulance> v; v.reserve(queue.size());
    Ambulance temp;
    for (int i = 0; i < queue.size(); ++i) if (queue.getAt(i,temp)) v.push_back(temp);

    sort(v.begin(), v.end(), [](const Ambulance& a, const Ambulance& b){
        if(a.shift!=b.shift) return a.shift < b.shift; return a.id < b.id;
    });

    cout << "\n================ Ambulance Schedule ================\n";
    cout << "| " << setw(4) << "ID"
         << " | " << setw(12) << "Plate"
         << " | " << setw(20) << "Driver"
         << " | " << setw(9) << "Shift" << " |\n";
    cout << "----------------------------------------------------\n";

    for (const auto& a : v) {
        string shiftName = (a.shift == 0 ? "Morning" : (a.shift == 1 ? "Afternoon" : "Midnight"));
        cout << "| " << setw(4) << a.id
             << " | " << setw(12) << a.plate
             << " | " << setw(20) << a.driverName
             << " | " << setw(9) << shiftName << " |\n";
    }
    cout << "====================================================\n";
}

// ==============================================
//   Menu
// ==============================================

void ambulanceMenu(AmbulanceManager& manager) {
    manager.loadFromFile();
    int choice;

    while (true) {
        cout << "\n========== AMBULANCE DISPATCHER MENU ==========\n";
        cout << "1. Register Ambulance\n";
        cout << "2. Rotate Ambulance Shift (all ambulances shift to next time-slot)\n";
        cout << "3. Display Ambulance Schedule (sorted by shift)\n";
        cout << "4. Show Queue Order (non-sorted, current queue order)\n";
        cout << "0. Back to Main Menu\n";
        cout << "===============================================\n";
        cout << "Enter choice: ";

        if (!(cin >> choice)) { 
            cout << "[ERROR] Invalid input. Numbers only.\n"; 
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            continue; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: manager.registerAmbulance(); break;
            case 2: manager.rotateShift(); break;
            case 3: manager.displaySchedule(); break;
            case 4: manager.getQueue().display(); break;
            case 0: cout << "Returning to main menu...\n"; manager.saveToFile(); return;
            default: cout << "[ERROR] Invalid choice. Try again.\n";
        }
    }
}

int main() {
    AmbulanceManager manager;
    ambulanceMenu(manager);
    return 0;
}