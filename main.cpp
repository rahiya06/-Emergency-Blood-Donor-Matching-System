```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

class Donor {
private:
    int id;
    string name;
    int age;
    string bloodGroup;
    string phone;
    string city;
    bool available;

public:
    Donor() {
        id = 0;
        age = 0;
        available = true;
    }

    void input(int donorId) {
        id = donorId;

        cout << "\nEnter donor name: ";
        getline(cin, name);

        while (true) {
            cout << "Enter age: ";

            if (cin >> age && age >= 18 && age <= 100) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }

            cout << "Please enter a valid age (18-100).\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cout << "Enter blood group (A+, A-, B+, B-, AB+, AB-, O+, O-): ";
        getline(cin, bloodGroup);

        transform(bloodGroup.begin(), bloodGroup.end(),
                  bloodGroup.begin(),
                  [](unsigned char c) { return toupper(c); });

        cout << "Enter phone number: ";
        getline(cin, phone);

        cout << "Enter city: ";
        getline(cin, city);

        char choice;
        cout << "Is the donor currently available? (Y/N): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        available = (toupper(static_cast<unsigned char>(choice)) == 'Y');
    }

    void display() const {
        cout << "\nDonor ID: " << id
             << "\nName: " << name
             << "\nAge: " << age
             << "\nBlood Group: " << bloodGroup
             << "\nPhone: " << phone
             << "\nCity: " << city
             << "\nAvailability: "
             << (available ? "Available" : "Not Available")
             << "\n-----------------------------\n";
    }

    bool matchesBloodGroup(const string& group) const {
        return bloodGroup == group;
    }

    bool isAvailable() const {
        return available;
    }

    int getId() const {
        return id;
    }

    void save(ofstream& file) const {
        file << id << '|'
             << name << '|'
             << age << '|'
             << bloodGroup << '|'
             << phone << '|'
             << city << '|'
             << available << '\n';
    }

    bool load(ifstream& file) {
        string line;

        if (!getline(file, line, '|'))
            return false;

        try {
            id = stoi(line);
        } catch (...) {
            return false;
        }

        if (!getline(file, name, '|'))
            return false;

        string ageText;

        if (!getline(file, ageText, '|'))
            return false;

        try {
            age = stoi(ageText);
        } catch (...) {
            return false;
        }

        if (!getline(file, bloodGroup, '|'))
            return false;

        if (!getline(file, phone, '|'))
            return false;

        if (!getline(file, city, '|'))
            return false;

        string availabilityText;

        if (!getline(file, availabilityText))
            return false;

        available = (availabilityText == "1");

        return true;
    }
};

class BloodDonorSystem {
private:
    static const int MAX_DONORS = 500;
    Donor donors[MAX_DONORS];
    int count;

    const string filename = "donors.txt";

    void loadDonors() {
        ifstream file(filename.c_str());

        if (!file.is_open())
            return;

        Donor donor;

        while (count < MAX_DONORS && donor.load(file)) {
            donors[count] = donor;
            count++;
        }

        file.close();
    }

    void saveDonors() const {
        ofstream file(filename.c_str());

        if (!file.is_open()) {
            cout << "Error: Could not save donor records.\n";
            return;
        }

        for (int i = 0; i < count; i++) {
            donors[i].save(file);
        }

        file.close();
    }

    string getBloodGroup() {
        string group;

        cout << "Enter blood group to search: ";
        getline(cin, group);

        transform(group.begin(), group.end(),
                  group.begin(),
                  [](unsigned char c) { return toupper(c); });

        return group;
    }

public:
    BloodDonorSystem() {
        count = 0;
        loadDonors();
    }

    void addDonor() {
        if (count >= MAX_DONORS) {
            cout << "\nDonor storage is full!\n";
            return;
        }

        Donor donor;
        donor.input(count + 1);

        donors[count] = donor;
        count++;

        saveDonors();

        cout << "\nDonor added successfully!\n";
    }

    void searchDonor() {
        // Check whether any records exist before asking for input.
        if (count == 0) {
            cout << "\nNo donor records found!\n";
            return;
        }

        string group = getBloodGroup();
        bool found = false;

        for (int i = 0; i < count; i++) {
            if (donors[i].matchesBloodGroup(group)) {
                donors[i].display();
                found = true;
            }
        }

        if (!found) {
            cout << "\nNo donors found for blood group "
                 << group << "!\n";
        }
    }

    void displayAllDonors() const {
        if (count == 0) {
            cout << "\nNo donor records found!\n";
            return;
        }

        cout << "\n========== ALL DONORS ==========\n";

        for (int i = 0; i < count; i++) {
            donors[i].display();
        }
    }

    void searchAvailableDonors() {
        if (count == 0) {
            cout << "\nNo donor records found!\n";
            return;
        }

        string group = getBloodGroup();
        bool found = false;

        for (int i = 0; i < count; i++) {
            if (donors[i].matchesBloodGroup(group) &&
                donors[i].isAvailable()) {
                donors[i].display();
                found = true;
            }
        }

        if (!found) {
            cout << "\nNo available donors found for blood group "
                 << group << "!\n";
        }
    }

    void run() {
        int choice;

        while (true) {
            cout << "\n========================================\n";
            cout << "   EMERGENCY BLOOD DONOR MATCHING SYSTEM\n";
            cout << "========================================\n";
            cout << "1. Add New Donor\n";
            cout << "2. Search Donors by Blood Group\n";
            cout << "3. Display All Donors\n";
            cout << "4. Search Available Donors\n";
            cout << "5. Exit\n";
            cout << "========================================\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cout << "\nInvalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1:
                    addDonor();
                    break;

                case 2:
                    searchDonor();
                    break;

                case 3:
                    displayAllDonors();
                    break;

                case 4:
                    searchAvailableDonors();
                    break;

                case 5:
                    cout << "\nThank you for using the system!\n";
                    return;

                default:
                    cout << "\nInvalid choice! Please select 1-5.\n";
            }
        }
    }
};

int main() {
    BloodDonorSystem system;
    system.run();

    return 0;
}
```
