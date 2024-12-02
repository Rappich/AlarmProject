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
/* void listCustomers();
void addCustomer();
void modifyCustomer();
void deleteCustomer(); */
void showMenu();

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
            //listCustomers();
            break;
        case 2:
            //addCustomer();
            break;
        case 3:
            //modifyCustomer();
            break;
        case 4:
            //deleteCustomer();
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
                    component["name"],
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


/* //Anpassa
void addCustomer()
{
    Customer newCustomer;
    cout << "Enter Customer details:\n";
    cout << "Customer ID: ";
    cin >> newCustomer.customerID
    cin.ignore();
    cout << "First and last name: ";
    getline(cin, newCustomer.name);
    cout << "Alarm system adress: ";
    cin >> newCustomer.address;
    cin.ignore();
    cout << "Alarm component: ";
    getline(cin, newCustomer.type);
    cout << "Product name: ";
    getline(cin, newCustomer.productName);

    Customers.push_back(newCustomer);
    cout << "Customer added successfully.\n";
}

void modifyCustomer()
{
    int id;
    cout << "Enter the ID of the Customer to modify: ";
    cin >> id;

    for (auto &Customer : Customers)
    {
        if (Customer.id == id)
        {
            cout << "Enter new details (leave blank to keep current values):\n";
            cout << "Current First Name: " << Customer.first_name << "\nNew First name: ";
            cin.ignore();
            string newfirst_name;
            getline(cin, newfirst_name);
            if (!newfirst_name.empty())
                Customer.first_name = newfirst_name;

            cout << "Current Last Name: " << Customer.last_name << "\nNew Last name: ";
            string newlast_name;
            getline(cin, newlast_name);
            if (!newlast_name.empty())
                Customer.last_name = newlast_name;

            cout << "Current Birth Year: " << Customer.birth_year << "\nNew Birth Year: ";
            string newbirthyear;
            cin >> newbirthyear;
            if (!newbirthyear.empty())
                Customer.birth_year = stoi(newbirthyear);

            cout << "Current Email: " << Customer.email << "\nNew Email: ";
            string newemail;
            getline(cin, newemail);
            if (!newemail.empty())
                Customer.email = newemail;

            cout << "Current Phone: " << Customer.phone << "\nNew Phone: ";
            string newPhone;
            getline(cin, newPhone);
            if (!newPhone.empty())
                Customer.phone = newPhone;

            cout << "Customer details updated successfully.\n";
            return;
        }
    }

    cout << "No Customer found with ID " << id << ".\n";
}

void deleteCustomer()
{
    int id;
    cout << "Enter the ID of the Customer to delete: ";
    cin >> id;

    auto it = remove_if(Customers.begin(), Customers.end(), [id](const Customer &Customer)
                        { return Customer.id == id; });

    if (it != Customers.end())
    {
        Customers.erase(it, Customers.end());
        cout << "Customer deleted successfully.\n";
    }
    else
    {
        cout << "No Customer found with ID " << id << ".\n";
    }
}


 */