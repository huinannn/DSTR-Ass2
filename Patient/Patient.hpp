#ifndef PATIENT_HPP
#define PATIENT_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Patient {
    int id;
    string name;
    string condition;
    Patient* next;
};

class PatientQueue {
private:
    Patient* front;
    Patient* rear;
    int lastID; // tracks last assigned patient ID

public:
    PatientQueue() {
        front = nullptr;
        rear = nullptr;
        lastID = 0;
    }

    // =======================================================
    // LOAD PATIENTS FROM CSV
    // =======================================================
    void loadFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Patient.csv not found. Starting with empty queue.\n";
            return;
        }

        string line;
        getline(file, line); // Skip header

        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string idStr, name, condition;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, condition, ',');

            if (idStr.empty() || name.empty() || condition.empty())
                continue; // skip corrupted rows

            int id = stoi(idStr);

            // Track largest ID for auto increment
            if (id > lastID) lastID = id;

            // Load into queue WITHOUT saving
            admitPatient(id, name, condition, false);
        }

        file.close();
        cout << "Loaded Patient.csv (Last ID = " << lastID << ")\n";
    }

    // =======================================================
    // SAVE PATIENTS TO CSV
    // =======================================================
    void saveToCSV(const string& filename) {
        ofstream file(filename);

        file << "ID,Name,Condition\n";

        Patient* current = front;
        while (current != nullptr) {
            file << current->id << ","
                 << current->name << ","
                 << current->condition << "\n";
            current = current->next;
        }

        file.close();
    }

    // =======================================================
    // ENQUEUE PATIENT
    // =======================================================
    void admitPatient(int id, string name, string condition, bool save = true) {
        Patient* newPatient = new Patient{id, name, condition, nullptr};

        if (rear == nullptr) {
            front = rear = newPatient;
        } else {
            rear->next = newPatient;
            rear = newPatient;
        }

        if (save) saveToCSV("Patient.csv");
    }

    // =======================================================
    // AUTO-ID VERSION (used by menu)
    // =======================================================
    void admitPatientAuto(string name, string condition) {
        int newID = lastID + 1;
        lastID = newID;

        admitPatient(newID, name, condition, true);

        cout << "\nPatient admitted successfully.\n";
        cout << "Assigned ID: " << newID << "\n";
    }

    // =======================================================
    // DISCHARGE PATIENT (DEQUEUE)
    // =======================================================
    void dischargePatient() {
        if (front == nullptr) {
            cout << "No patients to discharge.\n";
            return;
        }

        // Preview patient before discharge
        cout << "\n=== Discharge Warning ===\n";
        cout << "The next patient to be discharged is:\n";
        cout << "ID: " << front->id
             << " | Name: " << front->name
             << " | Condition: " << front->condition << endl;

        cout << "Proceed with discharge? (Y/N): ";
        char confirm;
        cin >> confirm;

        if (confirm != 'Y' && confirm != 'y') {
            cout << "Discharge cancelled.\n";
            return;
        }

        // Execute discharge
        Patient* temp = front;
        front = front->next;
        if (front == nullptr) rear = nullptr;

        cout << "\nPatient discharged successfully.\n";

        delete temp;
        saveToCSV("Patient.csv");
    }

    // =======================================================
    // DISPLAY QUEUE (FIFO ORDER)
    // =======================================================
    void viewPatients() {
        if (front == nullptr) {
            cout << "\nNo patients in the queue.\n";
            return;
        }

        cout << "\n--- Current Patient Queue (FIFO) ---\n";
        Patient* current = front;

        while (current != nullptr) {
            cout << "ID: " << current->id
                 << " | Name: " << current->name
                 << " | Condition: " << current->condition << "\n";
            current = current->next;
        }

        cout << "-------------------------------------\n";
    }
};

#endif
