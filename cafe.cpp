#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
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

    void generateBillHTML(const string &filename) const
    {
        ofstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for writing." << TERM_RESET << endl;
            return;
        }

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
        file << "</body>\n</html>\n";

        file.close();
        cout << TERM_GREEN << "Bill generated successfully." << TERM_RESET << endl;
    }
};

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
            cout << "Enter Menu Item ID to add to the order: ";
            cin >> itemID;
            MenuItem *item = getMenuItemByID(itemID);
            if (item)
            {
                newOrder.addItem(*item);
            }
            else
            {
                cout << TERM_RED << "Item ID not found in the menu." << TERM_RESET << endl;
            }
            cout << "Add more items? (y/n): ";
            cin >> moreItems;
        } while (moreItems == 'y' || moreItems == 'Y');

        if (orderCount < MAX_ORDERS)
        {
            orders[orderCount++] = newOrder;
        }
        else
        {
            cout << TERM_RED << "Cannot add more orders. Maximum limit reached." << TERM_RESET << endl;
        }
    }

    void displayOrders() const
    {
        cout << "\nOrders:" << endl;
        for (int i = 0; i < orderCount; i++)
        {
            orders[i].displayOrder();
            cout << endl;
        }
    }

    void saveDataToFile(const string &filename) const
    {
        ofstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for writing." << TERM_RESET << endl;
            return;
        }

        file << name << '\n'
             << location << '\n'
             << menuItemCount << '\n';
        for (int i = 0; i < menuItemCount; i++)
        {
            menu[i].saveToFile(file);
        }
        file << orderCount << '\n';
        for (int i = 0; i < orderCount; i++)
        {
            orders[i].saveToFile(file);
        }

        file.close();
        cout << TERM_GREEN << "Data saved to file." << TERM_RESET << endl;
    }

    void loadDataFromFile(const string &filename)
    {
        ifstream file(filename);
        if (!file)
        {
            cout << TERM_RED << "Error opening file for reading." << TERM_RESET << endl;
            return;
        }

        getline(file, name);
        getline(file, location);
        file >> menuItemCount;
        file.ignore();
        for (int i = 0; i < menuItemCount; i++)
        {
            menu[i].loadFromFile(file);
        }
        file >> orderCount;
        file.ignore();
        for (int i = 0; i < orderCount; i++)
        {
            orders[i].loadFromFile(file);
        }

        file.close();
        cout << TERM_GREEN << "Data loaded from file." << TERM_RESET << endl;
    }

    void saveDataToCSV(const string &filename) const {
        ofstream file(filename);
        if (!file) {
            cout << TERM_RED << "Error opening file for writing." << TERM_RESET << endl;
            return;
        }

        file << "MenuItems:\n";
        for (int i = 0; i < menuItemCount; i++) {
            menu[i].saveToCSV(file);
        }
        file << "Orders:\n";
        for (int i = 0; i < orderCount; i++) {
            orders[i].saveToCSV(file);
        }

        file.close();
        cout << TERM_GREEN << "Data saved to CSV file." << TERM_RESET << endl;
    }
};

// Display Cafe Art
#include <graphics.h> // Include the graphics library

void displayCafeArt()
{
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    // Set background color
    setbkcolor(CYAN); // Set background color to Cyan
    cleardevice();    // Clear the screen with the background color

    // Draw a rectangle for the title background
    setfillstyle(SOLID_FILL, BLUE);                              // Blue fill color
    rectangle(50, 30, getmaxx() - 50, 100);                      // Draw rectangle
    floodfill((50 + getmaxx() - 50) / 2, (30 + 100) / 2, WHITE); // Fill it with color

    // Set text style for the title
    settextstyle(BOLD_FONT, HORIZ_DIR, 5); // Bold font, horizontal direction, size 5
    setcolor(WHITE);                       // Set text color to White
    outtextxy(150, 50, "ADV.CAFE");        // Display the title

    // Draw a decorative border
    setcolor(YELLOW);
    rectangle(10, 10, getmaxx() - 10, getmaxy() - 10); // Draw a border

    // Draw some decorative shapes
    setfillstyle(SOLID_FILL, RED);
    circle(getmaxx() - 100, getmaxy() - 100, 50);        // Draw a circle at the bottom right
    floodfill(getmaxx() - 100, getmaxy() - 100, YELLOW); // Fill it with color

    setfillstyle(SOLID_FILL, GREEN);
    ellipse(100, getmaxy() - 100, 0, 360, 60, 30); // Draw an ellipse at the bottom left
    floodfill(100, getmaxy() - 100, YELLOW);       // Fill it with color

    // Add more graphical elements if needed
    // For example, you could add lines, more shapes, or even images

    delay(5000);  // Delay to display the art
    closegraph(); // Close the graphics mode
}

// Main Function
int main()
{
    Cafe myCafe;
    myCafe.name = "The Cozy Cafe";
    myCafe.location = "123 Coffee Lane";

    // Initialize Menu Items
    if (myCafe.menuItemCount == 0)
    {
        myCafe.addMenuItem(MenuItem(1, "Espresso", 2.50, "drink"));
        myCafe.addMenuItem(MenuItem(2, "Cappuccino", 3.00, "drink"));
        myCafe.addMenuItem(MenuItem(3, "Blueberry Muffin", 2.00, "veg-food"));
        myCafe.addMenuItem(MenuItem(4, "Burger", 3.50, "nonveg-food"));
        myCafe.addMenuItem(MenuItem(5, "Cake", 3.50, "nonveg-food"));
        myCafe.addMenuItem(MenuItem(6, "Pizza", 6.50, "veg-food"));
        myCafe.addMenuItem(MenuItem(7, "Samosa", 1.50, "veg-food"));
        myCafe.addMenuItem(MenuItem(8, "Mutton", 15.50, "nonveg-food"));
        myCafe.addMenuItem(MenuItem(9, "Tea", 1.50, "drink"));
        myCafe.addMenuItem(MenuItem(10, "Coke", 5.50, "drink"));
    }

    // Display Cafe Art
    displayCafeArt();

    // Main Menu Loop
    int choice;
    do
    {
        cout << TERM_BLUE << "Cafe Management System" << TERM_RESET << endl;
        cout << "1. Add Menu Item" << endl;
        cout << "2. Display Menu" << endl;
        cout << "3. Take Order" << endl;
        cout << "4. Display Orders" << endl;
        cout << "5. Save Data to File" << endl;
        cout << "6. Load Data from File" << endl;
        cout << "7. Save Data to CSV" << endl;
        cout << "8. Generate Bill" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice)
        {
        case 1:
        {
            MenuItem item;
            cout << "Enter Item ID: ";
            cin >> item.itemID;
            cin.ignore();
            cout << "Enter Item Name: ";
            getline(cin, item.itemName);
            cout << "Enter Price: ";
            cin >> item.price;
            cin.ignore();
            cout << "Enter Category (e.g., veg-food, nonveg-food, drink): ";
            getline(cin, item.category);
            myCafe.addMenuItem(item);
            break;
        }
        case 2:
            myCafe.displayMenu();
            break;
        case 3:
            myCafe.takeOrder();
            break;
        case 4:
            myCafe.displayOrders();
            break;
        case 5:
        {
            string filename;
            cout << "Enter filename to save data: ";
            getline(cin, filename);
            myCafe.saveDataToFile(filename);
            break;
        }
        case 6:
        {
            string filename;
            cout << "Enter filename to load data: ";
            getline(cin, filename);
            myCafe.loadDataFromFile(filename);
            break;
        }
        case 7:
        {
            string filename;
            cout << "Enter filename to save data to CSV: ";
            getline(cin, filename);
            myCafe.saveDataToCSV(filename);
            break;
        }
        case 8:
        {
            int orderID;
            string filename;
            cout << "Enter Order ID for the bill: ";
            cin >> orderID;
            cin.ignore();
            cout << "Enter filename to save the bill: ";
            getline(cin, filename);

            bool found = false;
            for (int i = 0; i < myCafe.orderCount; i++)
            {
                if (myCafe.orders[i].orderID == orderID)
                {
                    myCafe.orders[i].generateBillHTML(filename);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << TERM_RED << "Order ID not found." << TERM_RESET << endl;
            }
            break;
        }
        case 9:
            cout << TERM_GREEN << "Exiting the system." << TERM_RESET << endl;
            break;
        default:
            cout << TERM_RED << "Invalid choice. Please try again." << TERM_RESET << endl;
            break;
        }
    } while (choice != 9);

    return 0;
}
