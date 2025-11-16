#ifndef MEDICAL_HPP
#define MEDICAL_HPP

#include <string>

struct Supply {
    std::string type;
    int quantity;
    std::string batch;
};

class MedicalSupplyManager {
private:
    static const int MAX_SUPPLIES = 100;   // change this if needed
    Supply supplies[MAX_SUPPLIES];        // array-based stack
    int top;                              // index of last element; -1 = empty

    // CSV file path (relative to where the program runs)
    const std::string CSV_PATH = "Medical/Medical.csv";

    void loadFromCSV();   // read existing data from CSV into stack
    void saveToCSV();     // write current stack to CSV

public:
    MedicalSupplyManager();

    // Core functionalities
    void addSupply();         // 1. Add Supply Stock
    void useLastSupply();     // 2. Use 'Last Added' Supply
    void viewSupplies() const;// 3. View Current Supplies

    // Helper functions
    bool isFull() const;
    bool isEmpty() const;
};

// Function to handle menu for this module
void medicalSupplyMenu(MedicalSupplyManager &manager);

#endif // MEDICAL_HPP
