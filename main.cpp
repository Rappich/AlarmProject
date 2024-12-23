#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <map>
#include <algorithm>

using json = nlohmann::json;
using namespace std;

struct User
{
    int userID;
    int keyCode;
    string rfid;
    string securityPhrase;
};
struct AlarmComponent
{
    string type; // Type of alarmsystem, eg. smoke detector
    string productName; // Product name 
    int componentId; 
};

struct AlarmSystem
{   
    string address;
    vector<User> users;
    vector<AlarmComponent> alarmComponents;
};
struct Customer
{
    int customerID;
    string name;
    vector<AlarmSystem> alarmSystem;
};


map<int, Customer> Customers;

const string dataFilePath = "example.json";

void loadData();
void saveData();
void displayCustomersAndUsers();
void addCustomer();
/* void modifyCustomer(); */
void deleteCustomer(); 
void showMenu();
int currentCustomerID = 1;
int currentUserID = 1;
int main() 
{
    loadData();
    

    int choice;
    do
    {
        
        showMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            displayCustomersAndUsers();
            break;
        case 2:
            addCustomer();
            break;
        case 3:
/*             modifyCustomer(); */
            break;
        case 4:
            deleteCustomer();
            break;
        case 5:
            saveData();
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

return 0;

}

void loadData() {
    ifstream file(dataFilePath);
    if (!file) 
    {
        cout << "No existing data file found. Starting with an empty database.\n";
        return;
    }

    json jsonData;
    file >> jsonData;

    int maxCustomerID = 0;
    int maxUserID = 0;

    for (const auto& item : jsonData) {
        int customerId = item["customerId"];
        string customerName = item["name"];

        vector<AlarmSystem> alarmSystems;
        for (const auto& alarmSystem : item["alarmSystems"]) 
        {
            string address = alarmSystem["address"];

            vector<User> users;
            for (const auto& user : alarmSystem["users"]) 
            {
                int userId = user["userId"];
                users.push_back(User{
                    userId,
                    user["keyCode"],
                    user["rfid"],
                    user["securityPhrase"]
                });

                maxUserID = max(maxUserID, userId);
            }

            vector<AlarmComponent> alarmComponents;
            for (const auto& component : alarmSystem["alarmComponents"]) {
                alarmComponents.push_back(AlarmComponent{
                    component["type"],
                    component["productName"],
                    component["componentId"]
                });
            }

            alarmSystems.push_back(AlarmSystem{
                address,
                users,
                alarmComponents
            });
        }

        Customers.emplace(customerId, Customer{
            customerId,
            customerName,
            alarmSystems
        });

        maxCustomerID = max(maxCustomerID, customerId);
    }

    currentCustomerID = maxCustomerID + 1;
    currentUserID = maxUserID + 1;

    cout << "Data successfully loaded from " << dataFilePath << ".\n";
}



void showMenu()
{
    cout << "\n===== Library Database Menu =====\n";
    cout << "1. List all Customers\n";
    cout << "2. Add a new Customer\n";
    cout << "3. Modify an existing Customer\n";
    cout << "4. Delete a Customer\n";
    cout << "5. Exit\n";
    cout << "Choose an option: ";
}

void saveData() 
{
    ofstream file(dataFilePath);
    if (!file) 
    {
        cerr << "Error opening file for writing: " << dataFilePath << endl;
        return;
    }

    json jsonData;

    for (const auto& Pair : Customers) 
    {
        json customerJson;
        Customer customer = Pair.second;
        customerJson["customerId"] = customer.customerID;
        customerJson["name"] = customer.name;

        json alarmSystemsJson = json::array();
        for (const auto& alarmSystem : customer.alarmSystem) 
        {
            json alarmSystemJson;
            alarmSystemJson["address"] = alarmSystem.address;

            json usersJson = json::array();
            for (const auto& user : alarmSystem.users) 
            {
                usersJson.push_back
                ({
                    {"userId", user.userID},
                    {"keyCode", user.keyCode},
                    {"rfid", user.rfid},
                    {"securityPhrase", user.securityPhrase}
                });
            }

            json componentsJson = json::array();
            for (const auto& component : alarmSystem.alarmComponents) 
            {
                componentsJson.push_back
                ({
                    {"type", component.type},
                    {"name", component.productName},
                    {"componentId", component.componentId}
                });
            }

            alarmSystemJson["users"] = usersJson;
            alarmSystemJson["alarmComponents"] = componentsJson;

            alarmSystemsJson.push_back(alarmSystemJson);
        }

        customerJson["alarmSystems"] = alarmSystemsJson;

        jsonData.push_back(customerJson);
    }

    file << jsonData.dump(4);
    file.close();

    cout << "Data successfully saved to " << dataFilePath << ".\n";
}



void addCustomer() 
{
    cout << "Do you want to add a completely new customer or just a user?\n";
    cout << "1. Add a new user to an existing customer\n";
    cout << "2. Add a new customer\n";
    cout << "3. Add both customer and user\n";
    int menuChoice;
    cin >> menuChoice;
    cin.ignore(); 

    switch (menuChoice) 
    {
    case 1: 
    {
        cout << "Which of the following Customers would you like to add a user to:\n";

        for (const auto& pair : Customers) 
        {
            cout << pair.first << ". " << pair.second.name << endl;
        }

        int customerID;
        cin >> customerID;
        cin.ignore(); 
        auto it = Customers.find(customerID);
        if (it == Customers.end()) 
        {
            cout << "Invalid Customer ID. Please try again.\n";
            break;
        }

        Customer& customer = it->second;

        if (customer.alarmSystem.empty()) 
        {
            cout << "No alarm systems available for this customer. Would you like to add one? (y/n): ";
            char choice;
            cin >> choice;
            cin.ignore(); 

            if (choice == 'y' || choice == 'Y') 
            {
                AlarmSystem newAlarmSystem;
                cout << "Enter Alarm System Address: ";
                getline(cin, newAlarmSystem.address);

                customer.alarmSystem.push_back(newAlarmSystem);
                cout << "Alarm system added successfully.\n";
            } else 
            {
                cout << "No alarm system added. Returning to menu.\n";
                break;
            }
        }

        cout << "Select an Alarm System by index:\n";
        for (int i = 0; i < customer.alarmSystem.size(); ++i) 
        {
            cout << i + 1 << ". Address: " << customer.alarmSystem[i].address << endl;
        }

        int alarmIndex;
        cin >> alarmIndex;
        cin.ignore(); 

        if (alarmIndex < 1 || alarmIndex > customer.alarmSystem.size()) 
        {
            cout << "Invalid choice. Please try again.\n";
            break;
        }

        AlarmSystem& selectedAlarmSystem = customer.alarmSystem[alarmIndex - 1];

        User newUser;
        newUser.userID = currentUserID++;
        cout << "Enter User Key Code: ";
        cin >> newUser.keyCode;
        cin.ignore();
        cout << "Enter User RFID: ";
        getline(cin, newUser.rfid);
        cout << "Enter User Security Phrase: ";
        getline(cin, newUser.securityPhrase);

        selectedAlarmSystem.users.push_back(newUser);
        cout << "User added successfully with UserID " << newUser.userID << ".\n";
        break;
}
        case 2: 
        {
            Customer newCustomer;
            newCustomer.customerID = currentCustomerID++;
            cout << "Enter Customer Name: ";
            getline(cin, newCustomer.name);

            Customers[newCustomer.customerID] = newCustomer;
            cout << "Customer added successfully with ID " << newCustomer.customerID << ".\n";
            break;
        }

        case 3: 
        {
            Customer newCustomer;
            newCustomer.customerID = currentCustomerID++;
            cout << "Enter Customer Name: ";
            getline(cin, newCustomer.name);

            AlarmSystem newAlarmSystem;
            cout << "Enter Alarm System Address: ";
            getline(cin, newAlarmSystem.address);

            User newUser;
            newUser.userID = currentUserID++;
            cout << "Enter User Key Code: ";
            cin >> newUser.keyCode;
            cin.ignore();

            cout << "Enter User RFID: ";
            getline(cin, newUser.rfid);
            cout << "Enter User Security Phrase: ";
            getline(cin, newUser.securityPhrase);

            newAlarmSystem.users.push_back(newUser);
            newCustomer.alarmSystem.push_back(newAlarmSystem);

            Customers[newCustomer.customerID] = newCustomer;

            cout << "Customer and user added successfully!\n";
            break;
        }

        default:
            cout << "Invalid option. Please try again.\n";
            break;
    }
}


void displayCustomersAndUsers()
{
    if (Customers.empty()) 
    {
        cout << "No customers found.\n";
        return;
    }

    cout << "\n===== Customer and User List =====\n";
    
        for (const auto& pair : Customers) 
    {
        const Customer& customer = pair.second;
        cout << "Customer ID: " << customer.customerID 
             << ", Name: " << customer.name << endl;

        for (const auto& alarmSystem : customer.alarmSystem) 
        {
            cout << "  Alarm System Address: " << alarmSystem.address << endl;

            for (const auto& user : alarmSystem.users) 
            {
                cout << "  User ID: " << user.userID 
                     << ", RFID: " << user.rfid 
                     << ", Security Phrase: " << user.securityPhrase << endl;
            }

            for (const auto& component : alarmSystem.alarmComponents) 
            {
                cout << "    Component: " << component.type 
                     << ", Product Name: " << component.productName << endl;
            }
        }
        cout << "----------------------------\n";
    }
}


/* void modifyCustomer()
{
    int customerID;
    cout << "Enter the Customer ID to modify: ";
    cin >> customerID;
    cin.ignore();

    auto customerIt = Customers.find(customerID);
    if (customerIt != Customers.end()) 
    {
        Customer& customer = customerIt->second;

        cout << "Customer details found for ID " << customerID << ":\n";
        cout << "Current Name: " << customer.name << "\n";
        
        cout << "Enter new Name (leave blank to keep current): ";
        string newName;
        getline(cin, newName);
        if (!newName.empty())
            customer.name = newName;

        for (auto& alarmSystem : customer.alarmSystem) 
        {
            cout << "Current Alarm System Address: " << alarmSystem.address << "\n";
            cout << "Enter new Alarm System Address (leave blank to keep current): ";
            string newAddress;
            getline(cin, newAddress);
            if (!newAddress.empty())
                alarmSystem.address = newAddress;

            for (auto& user : alarmSystem.users) 
            {
                cout << "Current User ID: " << user.userID << ", RFID: " << user.rfid << "\n";
                cout << "Modify RFID for this User (leave blank to keep current): ";
                string newRfid;
                getline(cin, newRfid);
                if (!newRfid.empty())
                    user.rfid = newRfid;

                cout << "Modify Security Phrase for this User (leave blank to keep current): ";
                string newSecurityPhrase;
                getline(cin, newSecurityPhrase);
                if (!newSecurityPhrase.empty())
                    user.securityPhrase = newSecurityPhrase;
            }
        }
        cout << "Customer ID " << customerID << " updated successfully.\n";
    } 
    else 
    {
        cout << "No Customer found with ID " << customerID << ".\n";
    }
} */

void deleteCustomer() 
{
    int customerID;
    cout << "Enter the Customer ID to delete or manage: ";
    cin >> customerID;

    auto customerIt = Customers.find(customerID);
    if (customerIt == Customers.end()) 
    {
        cout << "Customer with ID " << customerID << " not found.\n";
        return;
    }

    cout << "Do you want to delete:\n";
    cout << "1. The entire customer\n";
    cout << "2. A specific user from the customer\n";
    int choice;
    cin >> choice;

    switch (choice) 
    {
        case 1: 
        { 
            Customers.erase(customerIt);
            cout << "Customer with ID " << customerID << " deleted successfully.\n";
            break;
        }

        case 2: 
        {
            Customer& customer = customerIt->second;

            cout << "Enter the User ID to delete: ";
            int userID;
            cin >> userID;

            bool userDeleted = false;

            for (auto it = customer.alarmSystem.begin(); it != customer.alarmSystem.end(); ) 
            {
                auto& alarmSystem = *it;

                auto userIt = find_if(alarmSystem.users.begin(), alarmSystem.users.end(),
                    [userID](const User& user) { return user.userID == userID; });

                if (userIt != alarmSystem.users.end()) 
                {
                    alarmSystem.users.erase(userIt);
                    userDeleted = true;
                    cout << "User with ID " << userID << " deleted successfully.\n";
                }

                if (alarmSystem.users.empty()) 
                {
                    it = customer.alarmSystem.erase(it);
                } else {
                    ++it;
                }
            }

            if (!userDeleted) 
            {
                cout << "User with ID " << userID << " not found for Customer ID " << customerID << ".\n";
            }
            break;
        }

        default:
            cout << "Invalid choice. Returning to menu.\n";
            break;
    }
}
