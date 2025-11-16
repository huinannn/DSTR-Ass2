#include "Medical.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>   // for formatting output

using namespace std;

// ===============================
// Constructor
// ===============================
MedicalSupplyManager::MedicalSupplyManager() : top(-1) {
    loadFromCSV();   // load existing data when object is created
}

// ===============================
// Helper functions
// ===============================
bool MedicalSupplyManager::isFull() const {
    return top == MAX_SUPPLIES - 1;
}

bool MedicalSupplyManager::isEmpty() const {
    return top == -1;
}

// ===============================
// SAFE INPUT FUNCTIONS
// ===============================

// Get a non-empty string
string getNonEmptyString(const string &prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!input.empty())
            return input;

        cout << "[ERROR] Input cannot be empty. Please try again.\n";
    }
}

// Get a positive integer
int getPositiveInt(const string &prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(1000, '\n');  // clear buffer
            return value;
        }

        cout << "[ERROR] Please enter a valid positive number.\n";

        cin.clear();                // clear error flag
        cin.ignore(1000, '\n');     // clear invalid input
    }
}

// Handle menu
int getMenuChoice(const string &prompt, int min, int max) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice && choice >= min && choice <= max) {
            cin.ignore(1000, '\n'); // clear leftover newline ONCE
            return choice;
        }

        cout << "[ERROR] Please enter a number between " 
             << min << " and " << max << ".\n";

        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ===============================
// Load from CSV into stack
// ===============================
void MedicalSupplyManager::loadFromCSV() {
    ifstream file(CSV_PATH.c_str());

    if (!file.is_open()) {
        // File does not exist yet — start with empty stack
        return;
    }

    string line;
    top = -1; // reset stack

    while (getline(file, line)) {
        if (line.empty())
            continue;

        stringstream ss(line);
        string type, quantityStr, batch;
        int quantity;

        // Simple CSV parsing: type,quantity,batch
        if (!getline(ss, type, ','))
            continue;
        if (!getline(ss, quantityStr, ','))
            continue;
        if (!getline(ss, batch, ','))
            batch = ""; // batch might be empty

        // convert quantity
        stringstream qss(quantityStr);
        qss >> quantity;
        if (qss.fail())
            continue; // invalid quantity, skip

        if (isFull()) {
            cout << "[Warning] Maximum supplies reached. Some records from CSV were ignored.\n";
            break;
        }

        top++;
        supplies[top].type = type;
        supplies[top].quantity = quantity;
        supplies[top].batch = batch;
    }

    file.close();
}

// ===============================
// Save stack to CSV
// ===============================
void MedicalSupplyManager::saveToCSV() {
    ofstream file(CSV_PATH.c_str()); // overwrite file

    if (!file.is_open()) {
        cout << "[Error] Unable to open CSV file for writing: " << CSV_PATH << endl;
        return;
    }

    for (int i = 0; i <= top; ++i) {
        file << supplies[i].type << ","
             << supplies[i].quantity << ","
             << supplies[i].batch << "\n";
    }

    file.close();
}

// ===============================
// 1. Add Supply Stock (PUSH)
// ===============================
void MedicalSupplyManager::addSupply() {
    if (isFull()) {
        cout << "Supply stack is full! Cannot add more supplies.\n";
        return;
    }

    cout << "\n=== Add Supply Stock ===\n";

    Supply s;

    s.type = getNonEmptyString("Enter supply type: ");
    s.quantity = getPositiveInt("Enter quantity: ");
    s.batch = getNonEmptyString("Enter batch number: ");

    supplies[++top] = s;
    saveToCSV();

    cout << "[INFO] Supply added successfully and saved.\n";
}

// ===============================
// 2. Use 'Last Added' Supply (POP)
// ===============================
void MedicalSupplyManager::useLastSupply() {
    if (isEmpty()) {
        cout << "No supplies available to use.\n";
        return;
    }

    cout << "\n=== Use Last Added Supply ===\n";
    cout << "Type  : " << supplies[top].type << "\n";
    cout << "Qty   : " << supplies[top].quantity << "\n";
    cout << "Batch : " << supplies[top].batch << "\n";

    string confirm;
    cout << "\nConfirm usage? (Y/N): ";
    cin >> confirm;

    if (confirm == "Y" || confirm == "y") {
        top--;
        saveToCSV();
        cout << "[INFO] Supply removed and file updated.\n";
    } else {
        cout << "[INFO] Cancelled. Supply not removed.\n";
    }
}

// ===============================
// 3. View Current Supplies
// ===============================
void MedicalSupplyManager::viewSupplies() const {
    cout << "\n=== Current Supplies (Top of Stack First) ===\n";

    if (isEmpty()) {
        cout << "No supplies available.\n";
        return;
    }

    cout << left << setw(5) << "No"
         << setw(20) << "Type"
         << setw(10) << "Quantity"
         << setw(15) << "Batch" << "\n";
    cout << string(50, '-') << "\n";

    // Show from top (last added) down to bottom
    int counter = 1;
    for (int i = top; i >= 0; --i) {
        cout << left << setw(5) << counter
             << setw(20) << supplies[i].type
             << setw(10) << supplies[i].quantity
             << setw(15) << supplies[i].batch << "\n";
        counter++;
    }
}

// ===============================
// Menu function for this module
// ===============================
void medicalSupplyMenu(MedicalSupplyManager &manager) {
    int choice;

    do {
        cout << "\n===== Medical Supply Manager =====\n";
        cout << "1. Add Supply Stock\n";
        cout << "2. Use 'Last Added' Supply\n";
        cout << "3. View Current Supplies\n";
        cout << "4. Back to Main Menu\n";

        choice = getMenuChoice("Enter your choice: ", 1, 4);

        switch (choice) {
            case 1:
                manager.addSupply();
                break;

            case 2:
                manager.useLastSupply();
                break;

            case 3:
                manager.viewSupplies();
                break;

            case 4:
                cout << "Returning to main menu...\n";
                break;
        }

    } while (choice != 4);
}
