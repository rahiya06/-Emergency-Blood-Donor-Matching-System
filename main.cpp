#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
using namespace std;

// Class to store donor information
class Donor
{
private:
    int id;
    string name;
    int age;
    string bloodGroup;
    string contact;
    string city;
    bool available;

public:
    // Constructor
    Donor(int i, string n, int a, string bg,
          string c, string ct, bool av)
    {
        id = i;
        name = n;
        age = a;
        bloodGroup = bg;
        contact = c;
        city = ct;
        available = av;
    }

    // Getter functions
    int getId() const
    {
        return id;
    }

    string getBloodGroup() const
    {
        return bloodGroup;
    }

    bool isAvailable() const
    {
        return available;
    }

    // Display donor details
    void display() const
    {
        cout << "\nDonor ID: " << id;
        cout << "\nName: " << name;
        cout << "\nAge: " << age;
        cout << "\nBlood Group: " << bloodGroup;
        cout << "\nContact: " << contact;
        cout << "\nCity: " << city;
        cout << "\nAvailability: "
             << (available ? "Available" : "Unavailable");
        cout << "\n-----------------------------\n";
    }

    // Save donor details to a file
    void saveToFile(ofstream &file) const
    {
        file << id << "|"
             << name << "|"
             << age << "|"
             << bloodGroup << "|"
             << contact << "|"
             << city << "|"
             << available << "\n";
    }

    // Load donor details from a file
    static bool readFromFile(ifstream &file, Donor &d)
    {
        string line;

        if (!getline(file, line))
            return false;

        stringstream ss(line);

        string idStr, name, ageStr, bg;
        string contact, city, avStr;

        if (!getline(ss, idStr, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, ageStr, '|') ||
            !getline(ss, bg, '|') ||
            !getline(ss, contact, '|') ||
            !getline(ss, city, '|') ||
            !getline(ss, avStr))
        {
            return false;
        }

        try
        {
            int id = stoi(idStr);
            int age = stoi(ageStr);
            bool available = (avStr == "1");

            d = Donor(id, name, age, bg,
                      contact, city, available);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    // Default constructor for loading records
    Donor() : id(0), age(0), available(false) {}
};


// Class to manage the donor system
class BloodDonorSystem
{
private:
    vector<Donor> donors;
    string filename;

    // Load saved records
    void loadDonors()
    {
        donors.clear();

        ifstream file(filename.c_str());

        if (!file)
            return;

        Donor d;

        while (Donor::readFromFile(file, d))
        {
            donors.push_back(d);
        }

        file.close();
    }

    // Save all records
    void saveDonors()
    {
        ofstream file(filename.c_str());

        if (!file)
        {
            cout << "Error saving donor records!\n";
            return;
        }

        for (size_t i = 0; i < donors.size(); i++)
        {
            donors[i].saveToFile(file);
        }

        file.close();
    }

    // Check whether a blood group is valid
    bool validBloodGroup(string bg)
    {
        string groups[] = {
            "A+", "A-", "B+", "B-",
            "AB+", "AB-", "O+", "O-"
        };

        for (int i = 0; i < 8; i++)
        {
            if (bg == groups[i])
                return true;
        }

        return false;
    }

    // Generate the next donor ID
    int generateID()
    {
        int maxID = 0;

        for (size_t i = 0; i < donors.size(); i++)
        {
            if (donors[i].getId() > maxID)
                maxID = donors[i].getId();
        }

        return maxID + 1;
    }

public:
    // Constructor
    BloodDonorSystem()
    {
        filename = "donors.txt";
        loadDonors();
    }

    // Register a new donor
    void addDonor()
    {
        string name, bg, contact, city;
        int age;
        char choice;

        cout << "\n--- Register New Donor ---\n";

        cout << "Enter donor name: ";
        getline(cin, name);

        if (name.empty() || name.find('|') != string::npos)
        {
            cout << "Invalid name!\n";
            return;
        }

        cout << "Enter age: ";

        if (!(cin >> age))
        {
            cout << "Invalid age!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (age < 18 || age > 100)
        {
            cout << "Please enter an age between 18 and 100.\n";
            return;
        }

        cout << "Enter blood group (A+, A-, B+, B-, AB+, AB-, O+, O-): ";
        getline(cin, bg);

        // Convert blood group to uppercase
        for (size_t i = 0; i < bg.length(); i++)
        {
            bg[i] = toupper(static_cast<unsigned char>(bg[i]));
        }

        if (!validBloodGroup(bg))
        {
            cout << "Invalid blood group!\n";
            return;
        }

        cout << "Enter contact number: ";
        getline(cin, contact);

        cout << "Enter city: ";
        getline(cin, city);

        if (contact.empty() || city.empty() ||
            contact.find('|') != string::npos ||
            city.find('|') != string::npos)
        {
            cout << "Invalid contact number or city!\n";
            return;
        }

        cout << "Is the donor currently available? (Y/N): ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        bool available =
            (choice == 'Y' || choice == 'y');

        int id = generateID();

        Donor d(id, name, age, bg,
                contact, city, available);

        donors.push_back(d);
        saveDonors();

        cout << "\nDonor registered successfully!";
        cout << "\nAssigned Donor ID: " << id << "\n";
    }

    // Display all donors
    void viewDonors()
    {
        loadDonors();

        cout << "\n--- Registered Donors ---\n";

        if (donors.empty())
        {
            cout << "No donor records found.\n";
            return;
        }

        for (size_t i = 0; i < donors.size(); i++)
        {
            donors[i].display();
        }
    }

    // Search for available donors
    void searchDonors()
    {
        string bg;

        cout << "\n--- Search Available Donors ---\n";

        cout << "Enter required blood group: ";
        getline(cin, bg);

        for (size_t i = 0; i < bg.length(); i++)
        {
            bg[i] = toupper(static_cast<unsigned char>(bg[i]));
        }

        if (!validBloodGroup(bg))
        {
            cout << "Invalid blood group!\n";
            return;
        }

        loadDonors();

        bool found = false;

        for (size_t i = 0; i < donors.size(); i++)
        {
            if (donors[i].getBloodGroup() == bg &&
                donors[i].isAvailable())
            {
                donors[i].display();
                found = true;
            }
        }

        if (!found)
        {
            cout << "\nNo available donors found for "
                 << bg << ".\n";
        }
    }

    // Main menu
    void menu()
    {
        int choice;

        do
        {
            cout << "\n================================\n";
            cout << " EMERGENCY BLOOD DONOR SYSTEM\n";
            cout << "================================\n";
            cout << "1. Register New Donor\n";
            cout << "2. View All Donors\n";
            cout << "3. Search Available Donors\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                cout << "Please enter a valid number.\n";
                cin.clear();
                cin.ignore(
                    numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
                case 1:
                    addDonor();
                    break;

                case 2:
                    viewDonors();
                    break;

                case 3:
                    searchDonors();
                    break;

                case 4:
                    cout << "Thank you for using the system!\n";
                    break;

                default:
                    cout << "Invalid choice! Try again.\n";
            }

        } while (choice != 4);
    }
};


// Main function
int main()
{
    BloodDonorSystem system;
    system.menu();

    return 0;
}