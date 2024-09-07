#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm> // Include for std::replace
#include <graphics.h>

using namespace std;

// Constants
const int MAX_MENU_ITEMS = 100;
const int MAX_ORDER_ITEMS = 50;
const int MAX_ORDERS = 100;

// Color Codes for Terminal
const string TERM_RESET = "\033[0m";
const string TERM_GREEN = "\033[32m";
const string TERM_YELLOW = "\033[33m";
const string TERM_RED = "\033[31m";
const string TERM_BLUE = "\033[34m";

// MenuItem Class
class MenuItem
{
public:
    int itemID;
    string itemName;
    double price;
    string category;

    MenuItem() : itemID(0), itemName(""), price(0.0), category("") {}

    MenuItem(int id, const string &name, double p, const string &cat)
        : itemID(id), itemName(name), price(p), category(cat) {}

    void displayItem() const
    {
        if (category == "drink")
        {
            cout << TERM_BLUE;
        }
        else if (category == "veg-food")
        {
            cout << TERM_GREEN;
        }
        else if (category == "nonveg-food")
        {
            cout << TERM_RED;
        }
        else
        {
            cout << TERM_YELLOW;
        }
        cout << itemID << ". " << itemName << " - $" << price << TERM_RESET << endl;
    }

    void saveToFile(ofstream &file) const
    {
        file << itemID << '\n'
             << itemName << '\n'
             << price << '\n'
             << category << '\n';
    }

    void loadFromFile(ifstream &file)
    {
        file >> itemID;
        file.ignore();
        getline(file, itemName);
        file >> price;
        file.ignore();
        getline(file, category);
    }

    void saveToCSV(ofstream &file) const
    {
        file << itemID << ',' << itemName << ',' << price << ',' << category << '\n';
    }
};

// Order Class
class Order
{
public:
    static const int MAX_ORDER_ITEMS = 50;
    int orderID;
    int tableNumber;
    string userName;
    MenuItem orderItems[MAX_ORDER_ITEMS];
    int itemCount;
    double totalAmount;
    void generateBillHTML(const string &filename) const;  // Declaration
    // other members...
    Order() : orderID(0), tableNumber(0), userName(""), itemCount(0), totalAmount(0.0) {}

    void addItem(const MenuItem &item)
    {
        if (itemCount < MAX_ORDER_ITEMS)
        {
            orderItems[itemCount++] = item;
            totalAmount += item.price;
        }
        else
        {
            cout << TERM_RED << "Cannot add more items to the order. Maximum limit reached." << TERM_RESET << endl;
        }
    }

    void displayOrder() const
    {
        cout << "Order ID: " << orderID << "\nTable Number: " << tableNumber << "\nUser Name: " << userName << "\nItems:\n";
        for (int i = 0; i < itemCount; i++)
        {
            cout << i + 1 << ". " << orderItems[i].itemName << " - $" << orderItems[i].price << endl;
        }
        cout << "Total Amount: $" << totalAmount << endl;
    }

    void saveToFile(ofstream &file) const
    {
        file << orderID << '\n'
             << tableNumber << '\n'
             << userName << '\n'
             << itemCount << '\n';
        for (int i = 0; i < itemCount; i++)
        {
            file << orderItems[i].itemID << ',' << orderItems[i].itemName << ',' << orderItems[i].price << ',' << orderItems[i].category << '\n';
        }
        file << totalAmount << '\n';
    }

    void loadFromFile(ifstream &file)
    {
        file >> orderID;
        file >> tableNumber;
        file.ignore();
        getline(file, userName);
        file >> itemCount;
        file.ignore();
        for (int i = 0; i < itemCount; i++)
        {
            orderItems[i].loadFromFile(file);
        }
        file >> totalAmount;
        file.ignore();
    }

    void saveToCSV(ofstream &file) const
    {
        file << orderID << ',' << tableNumber << ',' << userName << ',' << itemCount << '\n';
        for (int i = 0; i < itemCount; i++)
        {
            file << orderItems[i].itemID << ',' << orderItems[i].itemName << ',' << orderItems[i].price << ',' << orderItems[i].category << '\n';
        }
        file << totalAmount << '\n';
    }
};
#include "include/Order.h"// Include the header file for the class definition
void Order::generateBillHTML(const string &filename) const
{
    // Implementation of generateBillHTML
    ofstream file(filename);
    if (!file)
    {
        cout << TERM_RED << "Error opening file for writing." << TERM_RESET << endl;
        return;
    }

    // URL or text to encode in the QR code
    string qrCodeData = "OrderID: " + to_string(orderID) + ", TotalAmount: $" + to_string(totalAmount);
    // URL encode the qrCodeData
    string encodedData = qrCodeData;

    // Replace spaces and other special characters with URL encoding
    auto replaceAll = [](string& str, const string& from, const string& to) {
        size_t start_pos = 0;
        while ((start_pos = str.find(from, start_pos)) != string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
    };

    replaceAll(encodedData, " ", "%20");
    replaceAll(encodedData, ",", "%2C");
    replaceAll(encodedData, ":", "%3A");
    replaceAll(encodedData, "$", "%24");

    // Generate QR code URL using Google Chart API
    string qrCodeURL = "https://chart.googleapis.com/chart?chs=150x150&cht=qr&chl=" + encodedData;

    file << "<html>\n<head><title>Bill</title></head>\n<body>\n";
    file << "<h1>Bill for Order ID: " << orderID << "</h1>\n";
    file << "<p>Table Number: " << tableNumber << "</p>\n";
    file << "<p>User Name: " << userName << "</p>\n";
    file << "<ul>\n";
    for (int i = 0; i < itemCount; i++)
    {
        file << "<li>" << orderItems[i].itemName << " - $" << orderItems[i].price << "</li>\n";
    }
    file << "</ul>\n";
    file << "<p>Total Amount: $" << totalAmount << "</p>\n";

    // Include QR code image
    file << "<h2>QR Code:</h2>\n";
    file << "<img src=\"" << qrCodeURL << "\" alt=\"QR Code\">\n";

    file << "</body>\n</html>\n";

    file.close();
    cout << TERM_GREEN << "Bill generated successfully." << TERM_RESET << endl;
}
// Cafe Class
class Cafe
{
public:
    static const int MAX_MENU_ITEMS = 100;
    static const int MAX_ORDERS = 100;
    string name;
    string location;
    MenuItem menu[MAX_MENU_ITEMS];
    int menuItemCount;
    Order orders[MAX_ORDERS];
    int orderCount;

    Cafe() : name(""), location(""), menuItemCount(0), orderCount(0) {}

    void addMenuItem(const MenuItem &item)
    {
        if (menuItemCount < MAX_MENU_ITEMS)
        {
            menu[menuItemCount++] = item;
        }
        else
        {
            cout << TERM_RED << "Cannot add more items to the menu. Maximum limit reached." << TERM_RESET << endl;
        }
    }

    void displayMenu() const
    {
        cout << "\nMenu:" << endl;
        for (int i = 0; i < menuItemCount; i++)
        {
            menu[i].displayItem();
        }
    }

    MenuItem *getMenuItemByID(int id)
    {
        for (int i = 0; i < menuItemCount; i++)
        {
            if (menu[i].itemID == id)
            {
                return &menu[i];
            }
        }
        return nullptr;
    }

    void takeOrder()
    {
        Order newOrder;
        cout << "Enter Order ID: ";
        cin >> newOrder.orderID;
        cout << "Enter Table Number: ";
        cin >> newOrder.tableNumber;
        cout << "Enter User Name: ";
        cin.ignore();
        getline(cin, newOrder.userName);

        int itemID;
        char moreItems;
        do
        {
            cout << "Enter Item ID to add to the order: ";
            cin >> itemID;

            MenuItem *item = getMenuItemByID(itemID);
            if (item)
            {
                newOrder.addItem(*item);
                cout << "Item added to the order." << endl;
            }
            else
            {
                cout << TERM_RED << "Item not found in the menu." << TERM_RESET << endl;
            }

            cout << "Add more items to the order? (y/n): ";
            cin >> moreItems;
        } while (moreItems == 'y' || moreItems == 'Y');

        orders[orderCount++] = newOrder;
        cout << TERM_GREEN << "Order placed successfully." << TERM_RESET << endl;
    }

    void displayOrders() const
    {
        for (int i = 0; i < orderCount; i++)
        {
            orders[i].displayOrder();
            cout << "--------------------" << endl;
        }
    }

    void saveOrdersToFile(const string &filename) const
    {
        ofstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for writing." << TERM_RESET << endl;
            return;
        }

        file << orderCount << '\n';
        for (int i = 0; i < orderCount; i++)
        {
            orders[i].saveToFile(file);
        }

        file.close();
        cout << TERM_GREEN << "Orders saved to file successfully." << TERM_RESET << endl;
    }

    void loadOrdersFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for reading." << TERM_RESET << endl;
            return;
        }

        file >> orderCount;
        file.ignore();
        for (int i = 0; i < orderCount; i++)
        {
            orders[i].loadFromFile(file);
        }

        file.close();
        cout << TERM_GREEN << "Orders loaded from file successfully." << TERM_RESET << endl;
    }

    void saveMenuToFile(const string &filename) const
    {
        ofstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for writing." << TERM_RESET << endl;
            return;
        }

        file << menuItemCount << '\n';
        for (int i = 0; i < menuItemCount; i++)
        {
            menu[i].saveToFile(file);
        }

        file.close();
        cout << TERM_GREEN << "Menu saved to file successfully." << TERM_RESET << endl;
    }

    void loadMenuFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for reading." << TERM_RESET << endl;
            return;
        }

        file >> menuItemCount;
        file.ignore();
        for (int i = 0; i < menuItemCount; i++)
        {
            menu[i].loadFromFile(file);
        }

        file.close();
        cout << TERM_GREEN << "Menu loaded from file successfully." << TERM_RESET << endl;
    }
};

void displayMenu()
{
    cout << TERM_BLUE << "1. Display Menu" << TERM_RESET << endl;
    cout << TERM_BLUE << "2. Take Order" << TERM_RESET << endl;
    cout << TERM_BLUE << "3. Display Orders" << TERM_RESET << endl;
    cout << TERM_BLUE << "4. Save Orders to File" << TERM_RESET << endl;
    cout << TERM_BLUE << "5. Load Orders from File" << TERM_RESET << endl;
    cout << TERM_BLUE << "6. Save Menu to File" << TERM_RESET << endl;
    cout << TERM_BLUE << "7. Load Menu from File" << TERM_RESET << endl;
    cout << TERM_BLUE << "8. Generate HTML Bill" << TERM_RESET << endl;
    cout << TERM_BLUE << "0. Exit" << TERM_RESET << endl;
}

int main()
{
    Cafe cafe;
    int choice;

    do
    {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            cafe.displayMenu();
            break;
        case 2:
            cafe.takeOrder();
            break;
        case 3:
            cafe.displayOrders();
            break;
        case 4:
            cafe.saveOrdersToFile("orders.txt");
            break;
        case 5:
            cafe.loadOrdersFromFile("orders.txt");
            break;
        case 6:
            cafe.saveMenuToFile("menu.txt");
            break;
        case 7:
            cafe.loadMenuFromFile("menu.txt");
            break;
        case 8:
            // Generate HTML bill for the first order as an example
            if (cafe.orderCount > 0)
            {
                cafe.orders[0].generateBillHTML("bill.html");
            }
            else
            {
                cout << TERM_RED << "No orders to generate a bill for." << TERM_RESET << endl;
            }
            break;
        case 0:
            cout << TERM_GREEN << "Exiting..." << TERM_RESET << endl;
            break;
        default:
            cout << TERM_RED << "Invalid choice. Please try again." << TERM_RESET << endl;
            break;
        }
    } while (choice != 0);

    return 0;
}
