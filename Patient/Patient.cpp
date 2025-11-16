#include <iostream>
#include "Patient.hpp"
using namespace std;

int main() {
    PatientQueue pq;

    // Load existing data from CSV
    pq.loadFromCSV("Patient.csv");

    int choice;
    string name, condition;

    do {
        cout << "\n========== Patient Admission Menu ==========\n";
        cout << "1. Admit Patient\n";
        cout << "2. Discharge Patient\n";
        cout << "3. View Patient Queue\n";
        cout << "0. Exit\n";
        cout << "Choose option: ";
        cin >> choice;
        cin.ignore(); // prevents input skipping

        switch (choice) {
            case 1:
                cout << "\nEnter Patient Name: ";
                getline(cin, name);

                cout << "Enter Condition Type: ";
                getline(cin, condition);

                pq.admitPatientAuto(name, condition);
                break;

            case 2:
                pq.dischargePatient();
                break;

            case 3:
                pq.viewPatients();
                break;

            case 0:
                cout << "Saving data and exiting...\n";
                break;

            default:
                cout << "Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}
