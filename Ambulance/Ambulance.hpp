#ifndef AMBULANCE_HPP
#define AMBULANCE_HPP

#include <string>

struct Ambulance {
    int  id;
    char plate[20];
    char driverName[50];
    int  shift; // 0 = Morning, 1 = Afternoon, 2 = Midnight
};

const int MAX_AMBULANCES = 100;

class AmbulanceQueue {
private:
    Ambulance arr[MAX_AMBULANCES];
    int front;
    int rear;
    int count;

public:
    AmbulanceQueue();

    bool isEmpty() const;
    bool isFull() const;

    bool enqueue(const Ambulance& a);
    bool dequeue(Ambulance& removed);

    bool rotate();

    void display() const;

    int  size() const;
    bool getAt(int index, Ambulance& out) const;

    void clear();
};

class AmbulanceManager {
private:
    AmbulanceQueue queue;
    std::string    fileName;

public:
    explicit AmbulanceManager(const std::string& file = "Ambulance/Ambulance.csv");

    void loadFromFile();
    void saveToFile() const;

    void registerAmbulance();
    void rotateShift();
    void displaySchedule() const;

    bool idExists(int id) const;
    bool plateExists(const std::string& plate) const; // ✅ NEW
    int  generateNewID() const;

    AmbulanceQueue&       getQueue()       { return queue; }
    const AmbulanceQueue& getQueue() const { return queue; }
};

void ambulanceMenu(AmbulanceManager& manager);

#endif
