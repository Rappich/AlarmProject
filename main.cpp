#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include <map>

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
void modifyCustomer();
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
            modifyCustomer();
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

void loadData()
{
    ifstream file(dataFilePath);
    if (!file)
    {
        cout << "No existing data file found. Starting with an empty database.\n";
        return;
    }

    json jsonData;
    file >> jsonData;

        for (const auto& item : jsonData) 
        {
        int customerId = item["customerId"];
        string customerName = item["name"];

        vector<AlarmSystem> alarmSystems;

        for (const auto& alarmSystem : item["alarmSystems"]) 
        {
            string address = alarmSystem["address"];

            vector<User> users;
            for (const auto& user : alarmSystem["users"]) 
            {
                users.push_back(User
                {
                    user["userId"],
                    user["keyCode"],
                    user["rfid"],
                    user["securityPhrase"]
                });
            }

            vector<AlarmComponent> alarmComponents;
            for (const auto& component : alarmSystem["alarmComponents"]) 
            {
                alarmComponents.push_back(AlarmComponent
                {
                    component["type"],
                    component["ProductName"],
                    component["componentId"]
                });
            }

            alarmSystems.push_back(AlarmSystem
            {
                address,
                users,
                alarmComponents
            });
        }
        Customers.emplace(customerId, Customer
        {
            customerId,
            customerName,
            alarmSystems
        });
    }

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
    cout << "Do you want to add a completly new customer or just a user?\n";
    cout << "1. Add a new user\n";
    cout << "2. Add a new customer\n";
    cout << "3. Add both customer & user\n";
    int choice;
    int menuChoice;
    cin >> menuChoice;
    cin.ignore();
 switch (menuChoice) 
 {
    case 1: 
    {
        int i = 1;
        cout << "Which of the following Customers would you like to add a user to:\n";
        
        for (const auto& pair : Customers) 
        {
            cout << i << ". " << pair.second.name << endl;
            i++;
        }

        int choice;
        cin >> choice;
        cin.ignore();

        if (choice < 1 || choice > Customers.size()) 
        {
            cout << "Invalid choice. Please try again.\n";
            break;
        }

        User newUser;
        newUser.userID = currentUserID++;
        
        cout << "Enter User details:\n";
        cout << "Key Code: ";
        cin >> newUser.keyCode;
        cin.ignore();
        cout << "RFID: ";
        getline(cin, newUser.rfid);

        Customer& customer = Customers[choice];
        if (!customer.alarmSystem.empty()) 
        {
            customer.alarmSystem[0].users.push_back(newUser); 
        }
        cout << "User added successfully with UserID " << newUser.userID << ".\n";
        break;
    }

    case 2: 
    {
        Customer newCustomer;
        
        cout << "Enter Customer details:\n";
        newCustomer.customerID = currentCustomerID++;
        cout << "Customer ID: " << newCustomer.customerID << endl;

        cout << "Write the customer's first and last name / Company name: ";
        getline(cin, newCustomer.name);

        Customers.emplace(newCustomer.customerID, newCustomer);
        cout << "Customer added successfully.\n";
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


void modifyCustomer()
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
}

void deleteCustomer() 
{
    int customerID, userID;
    cout << "Enter the Customer ID to delete: ";
    cin >> customerID;

    auto customerIt = Customers.find(customerID);
    if (customerIt != Customers.end()) 
    {
        Customer& customer = customerIt->second;

        cout << "Enter the User ID to delete: ";
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

        if (!userDeleted) {
            cout << "User with ID " << userID << " not found for Customer ID " << customerID << ".\n";
        }
    } else {
        cout << "Customer with ID " << customerID << " not found.\n";
    }
}


