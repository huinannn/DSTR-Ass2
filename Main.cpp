#include <iostream>
using namespace std;

// Include all role modules
#include "Patient/Patient.hpp"
#include "Medical/Medical.hpp"
#include "Emergency/Emergency.hpp"
#include "Ambulance/Ambulance.hpp"

// ===============================
// MAIN MENU INPUT VALIDATION
// ===============================
int getValidatedInt(const string &prompt, int min, int max) {
    int value;

    while (true) {
        cout << prompt;

        // Ensure integer entered
        if (cin >> value) {
            // Clear leftover '\n'
            cin.ignore(1000, '\n');

            // Check range
            if (value >= min && value <= max) {
                return value;
            }

            cout << "[ERROR] Please enter a number between "
                 << min << " and " << max << ".\n";
        } else {
            // Non-integer entered
            cout << "[ERROR] Please enter a valid NUMBER.\n";
            cin.clear();               // reset fail state
            cin.ignore(1000, '\n');    // remove bad input
        }
    }
}

// ===============================
// MAIN PROGRAM
// ===============================
int main() {
    // Create one instance for each role
    // PatientManager patientManager;
    MedicalSupplyManager medicalManager;
    // EmergencyManager emergencyManager;
    // AmbulanceManager ambulanceManager;

    int choice;

    do {
        cout << "\n=========================================\n";
        cout << "      HOSPITAL PATIENT CARE SYSTEM\n";
        cout << "=========================================\n";
        cout << "1. Patient Admission Clerk\n";
        cout << "2. Medical Supply Manager\n";
        cout << "3. Emergency Department Officer\n";
        cout << "4. Ambulance Dispatcher\n";
        cout << "5. Exit Program\n";

        // VALIDATED menu choice (1–5)
        choice = getValidatedInt("Enter your choice: ", 1, 5);

        switch (choice) {
            case 1:
                // patientMenu(patientManager);
                break;

            case 2:
                medicalSupplyMenu(medicalManager);
                break;

            case 3:
                // emergencyMenu(emergencyManager);
                break;

            case 4:
                // ambulanceMenu(ambulanceManager);
                break;

            case 5:
                cout << "\nExiting system... Goodbye!\n";
                break;
        }

    } while (choice != 5);

    return 0;
}