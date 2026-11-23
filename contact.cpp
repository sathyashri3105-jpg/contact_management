
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
using namespace std;

struct Contact
{
    string id;
    string name;
    string email;
    string phone;
    string category;
    string address;
};

class ContactManagementSystem
{
private:
    vector<Contact> contacts;

    const string filename = "my_contacts.txt";
    const string sampleFile = "contact.txt";

    // Generate unique Contact ID
    void generateID(Contact &c)
    {
        int maxID = 0;

        for (const Contact &contact : contacts)
        {
            if (contact.id.find("CON-") == 0)
            {
                int number = stoi(contact.id.substr(4));

                if (number > maxID)
                    maxID = number;
            }
        }

        stringstream ss;
        ss << "CON-" << setw(4)
           << setfill('0') << maxID + 1;

        c.id = ss.str();
    }

    // Save contacts to file
    void saveContacts()
    {
        ofstream file(filename);

        if (!file.is_open())
        {
            cout << "Error saving contacts!\n";
            return;
        }

        for (const Contact &c : contacts)
        {
            file << c.id << "|"
                 << c.name << "|"
                 << c.email << "|"
                 << c.phone << "|"
                 << c.category << "|"
                 << c.address << "\n";
        }

        file.close();
    }

    
void loadContacts()
{
    ifstream file(filename);

    if (!file.is_open())
    {
        file.open(sampleFile);
    }

    if (!file.is_open())
    {
        cout << "Contact file not found!\n";
        return;
    }

    string line;

    while (getline(file, line))
    {
        // Convert every pipe separator into a comma
        for (char &ch : line)
        {
            if (ch == '|')
                ch = ',';
        }

        // Skip empty and separator lines
        if (line.find_first_not_of(", \t\r") == string::npos)
            continue;

        stringstream ss(line);
        vector<string> fields;
        string field;

        while (getline(ss, field, ','))
        {
            fields.push_back(field);
        }

        // Need at least 5 fields
        if (fields.size() < 5)
            continue;

        Contact c;

        c.id = fields[0];
        c.name = fields[1];
        c.email = fields[2];
        c.phone = fields[3];
        c.category = fields[4];

        if (fields.size() > 5)
            c.address = fields[5];
        else
            c.address = "";

        // Skip header and invalid rows
        if (c.id.find("CON-") != 0 || c.name.empty())
            continue;

        contacts.push_back(c);
    }

    file.close();

    cout << "Contacts loaded successfully: "
         << contacts.size() << "\n";
}


    // Find contact by name or phone
    int findContact(string keyword)
    {
        for (int i = 0; i < contacts.size(); i++)
        {
            if (contacts[i].name == keyword ||
                contacts[i].phone == keyword)
            {
                return i;
            }
        }

        return -1;
    }

public:

    ContactManagementSystem()
    {
        loadContacts();
    }

    // Add contact
    void addContact()
    {
        Contact c;

        cout << "\nEnter Name: ";
        getline(cin, c.name);

        cout << "Enter Email: ";
        getline(cin, c.email);

        cout << "Enter Phone: ";
        getline(cin, c.phone);

        cout << "Enter Category (Work/College/Personal): ";
        getline(cin, c.category);

        cout << "Enter Address: ";
        getline(cin, c.address);

        if (c.name.empty())
        {
            cout << "\nName cannot be empty!\n";
            return;
        }

        generateID(c);

        contacts.push_back(c);

        saveContacts();

        cout << "\nContact added successfully!\n";
        cout << "Contact ID: " << c.id << "\n";
    }

    // Display all contacts
    void displayContacts()
    {
        if (contacts.empty())
        {
            cout << "\nNo contacts available.\n";
            return;
        }

        cout << "\n========== ALL CONTACTS ==========\n";

        for (const Contact &c : contacts)
        {
            cout << "\nContact ID : " << c.id;
            cout << "\nName       : " << c.name;
            cout << "\nEmail      : " << c.email;
            cout << "\nPhone      : " << c.phone;
            cout << "\nCategory   : " << c.category;
            cout << "\nAddress    : " << c.address;

            cout << "\n----------------------------------\n";
        }
    }

    // Search contact
    void searchContact()
    {
        string keyword;

        cout << "\nEnter Name or Phone to search: ";
        getline(cin, keyword);

        int index = findContact(keyword);

        if (index == -1)
        {
            cout << "\nContact not found.\n";
            return;
        }

        Contact c = contacts[index];

        cout << "\nContact Found!\n";
        cout << "ID       : " << c.id << "\n";
        cout << "Name     : " << c.name << "\n";
        cout << "Email    : " << c.email << "\n";
        cout << "Phone    : " << c.phone << "\n";
        cout << "Category : " << c.category << "\n";
        cout << "Address  : " << c.address << "\n";
    }

    // Edit contact
    void editContact()
    {
        string keyword;

        cout << "\nEnter Name or Phone to edit: ";
        getline(cin, keyword);

        int index = findContact(keyword);

        if (index == -1)
        {
            cout << "\nContact not found.\n";
            return;
        }

        cout << "\nLeave a field empty to keep its existing value.\n";

        string value;

        cout << "Enter New Name: ";
        getline(cin, value);
        if (!value.empty())
            contacts[index].name = value;

        cout << "Enter New Email: ";
        getline(cin, value);
        if (!value.empty())
            contacts[index].email = value;

        cout << "Enter New Phone: ";
        getline(cin, value);
        if (!value.empty())
            contacts[index].phone = value;

        cout << "Enter New Category: ";
        getline(cin, value);
        if (!value.empty())
            contacts[index].category = value;

        cout << "Enter New Address: ";
        getline(cin, value);
        if (!value.empty())
            contacts[index].address = value;

        saveContacts();

        cout << "\nContact updated successfully!\n";
    }

    // Delete contact
    void deleteContact()
    {
        string keyword;

        cout << "\nEnter Name or Phone to delete: ";
        getline(cin, keyword);

        int index = findContact(keyword);

        if (index == -1)
        {
            cout << "\nContact not found.\n";
            return;
        }

        contacts.erase(contacts.begin() + index);

        saveContacts();

        cout << "\nContact deleted successfully!\n";
    }

    // Main menu
    void menu()
    {
        int choice;

        cout << "\nContact Management System\n";
        cout << "Loaded Contacts: " << contacts.size() << "\n";

        do
        {
            cout << "\n========== CONTACT BOOK ==========\n";
            cout << "1. Add Contact\n";
            cout << "2. Display All Contacts\n";
            cout << "3. Search Contact\n";
            cout << "4. Edit Contact\n";
            cout << "5. Delete Contact\n";
            cout << "0. Save and Exit\n";
            cout << "==================================\n";

            cout << "Enter your choice: ";

            if (!(cin >> choice))
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                cout << "\nInvalid input! Enter a number.\n";
                continue;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice)
            {
                case 1:
                    addContact();
                    break;

                case 2:
                    displayContacts();
                    break;

                case 3:
                    searchContact();
                    break;

                case 4:
                    editContact();
                    break;

                case 5:
                    deleteContact();
                    break;

                case 0:
                    saveContacts();
                    cout << "\nContacts saved. Exiting program...\n";
                    break;

                default:
                    cout << "\nInvalid choice! Try again.\n";
            }

        } while (choice != 0);
    }
};

int main()
{
    ContactManagementSystem cms;

    cms.menu();

    return 0;
}
