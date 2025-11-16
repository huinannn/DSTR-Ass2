#include <iostream>
#include <string>
#include <cstdlib>   // for system()
#include <limits>
#include <algorithm>

using namespace std;

// ===============================
// Function to get compile/run commands
// ===============================
void getCompileRunCommands(const string &role, string &compileCmd, string &runCmd) {
#ifdef _WIN32
    string exeExt = ".exe";
#else
    string exeExt = "";
#endif

    if (role == "patient") {
        compileCmd = "g++ Patient/Patient.cpp -o Patient" + exeExt;
        runCmd = "Patient" + exeExt;
    } else if (role == "medical") {
        compileCmd = "g++ Medical/Medical.cpp -o Medical" + exeExt;
        runCmd = "Medical" + exeExt;
    } else if (role == "emergency") {
        compileCmd = "g++ Emergency/Emergency.cpp -o Emergency" + exeExt;
        runCmd = "Emergency" + exeExt;
    } else if (role == "ambulance") {
        compileCmd = "g++ Ambulance/Ambulance.cpp -o Ambulance" + exeExt;
        runCmd = "Ambulance" + exeExt;
    }

#ifndef _WIN32
    runCmd = "./" + runCmd; // Unix-style prefix
#endif
}

// ===============================
// Main Program
// ===============================
int main() {
    while (true) {
        cout << "=========================================\n";
        cout << "      HOSPITAL PATIENT CARE SYSTEM\n";
        cout << "=========================================\n\n";

        cout << "Choose a module to run:\n";
        cout << "1. Patient Admission Clerk\n";
        cout << "2. Medical Supply Manager\n";
        cout << "3. Emergency Department Officer\n";
        cout << "4. Ambulance Dispatcher\n";
        cout << "5. Exit\n";

        string choiceStr;
        getline(cin, choiceStr);

        string role;
        if (choiceStr == "1") role = "patient";
        else if (choiceStr == "2") role = "medical";
        else if (choiceStr == "3") role = "emergency";
        else if (choiceStr == "4") role = "ambulance";
        else if (choiceStr == "5") break;
        else {
            cout << "[ERROR] Invalid choice. Try again.\n\n";
            continue;
        }

        // Compile & run selected module
        string compileCmd, runCmd;
        getCompileRunCommands(role, compileCmd, runCmd);

        cout << "\nCompiling " << role << " module...\n";
        int compileResult = system(compileCmd.c_str());
        if (compileResult != 0) {
            cout << "[ERROR] Compilation failed. Check file paths.\n\n";
            continue;
        }

        cout << "\nRunning " << role << " module...\n\n";
        int runResult = system(runCmd.c_str());
        if (runResult != 0) {
            cout << "[ERROR] Running module failed.\n\n";
        }

        // Ask if user wants to continue
        string cont;
        cout << "\nDo you want to return to main menu? (y/n): ";
        getline(cin, cont);
        transform(cont.begin(), cont.end(), cont.begin(), ::tolower);
        if (cont != "y" && cont != "yes") break;

        cout << "\n";
    }

    cout << "\nExiting system. Goodbye!\n";
    return 0;
}
