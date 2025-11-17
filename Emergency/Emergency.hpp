#ifndef EMERGENCY_HPP
#define EMERGENCY_HPP

#include <string>
using namespace std;

const string EMERGENCY_CSV = "Emergency/Emergency.csv";
const int MAX_EMERGENCY = 100;

struct Emergency {
    string id; 
    string name;
    string type;
    int priority;
};

// Emergency Manager (Priority Queue using Array)
class EmergencyManager {
private:
    Emergency cases[MAX_EMERGENCY];
    int size;                       
    int nextID;
    string generateID();

public:
    EmergencyManager();

    bool isFull() const;
    bool isEmpty() const;

    void loadFromCSV();
    void saveToCSV() const;

    void logCase();
    void processCritical();
    void viewCases() const;
};

void emergencyMenu(EmergencyManager &manager);

string EmptyVal(const string &prompt);
int PriorityVal(const string &prompt);
int MenuChoiceVal(const string &prompt, int min, int max);

#endif