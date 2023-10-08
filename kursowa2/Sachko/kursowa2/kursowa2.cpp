#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int FUEL_TYPES = 3;
const int COFFEE_TYPES = 3;

enum Menu {
    REFILL_FUEL = 1,
    READ_FUEL_DATA = 2,
    CHANGE_FUEL_PRICE = 3,
    REFILL_COFFEE = 4,
    READ_COFFEE_DATA = 5,
    CLEAR_FUEL_CACHE = 6,
    CLEAR_COFFEE_CACHE = 7,
    FUEL_SALES_STATISTICS = 8,
    DISPLAY_SALES = 9,
    QUIT = 0
};
enum Menu1 {
    ORDER_COFFEE = 1,
    FUEL_CAR = 2,
    QUIT1 = 0
};

struct Fuel
{
    string type;
    double price;
    int quantity;
};
struct Coffee
{
    string type;
    double price;
    int quantity;
};

void refillFuel(Fuel fuels[FUEL_TYPES]) {
    for (int i = 0; i < FUEL_TYPES; i++)
    {
        cout << "Enter quantity of " << fuels[i].type << " to refill: ";
        cin >> fuels[i].quantity;
    }
    cout << "Fuel refilled." << endl;
}

void readFuelData(const Fuel fuels[FUEL_TYPES]) {
    cout << "Fuel Availability:" << endl;
    for (int i = 0; i < FUEL_TYPES; i++)
    {
        cout << "Fuel Type: " << fuels[i].type << endl
            << "Quantity: " << fuels[i].quantity << " liters" << endl
            << "Price: $" << fuels[i].price << " per liter" << endl << endl;
    }
}

void changeFuelPrice(Fuel fuels[FUEL_TYPES]) {
    for (int i = 0; i < FUEL_TYPES; i++)
    {
        cout << "Enter new price for " << fuels[i].type << ": $";
        cin >> fuels[i].price;
    }
    cout << "Fuel prices changed." << endl;
}

void refillCoffee(Coffee coffees[COFFEE_TYPES]) {
    for (int i = 0; i < COFFEE_TYPES; i++)
    {
        cout << "Enter quantity of " << coffees[i].type << " to refill: ";
        cin >> coffees[i].quantity;
    }
    cout << "Coffee machine refilled." << endl;
}

void readCoffeeData(const Coffee coffees[COFFEE_TYPES]) {
    cout << "Coffee Menu:" << endl;
    for (int i = 0; i < COFFEE_TYPES; i++)
    {
        cout << "Coffee Type: " << coffees[i].type << endl;
        cout << "Quantity: " << coffees[i].quantity << " cups" << endl;
        cout << "Price: $" << coffees[i].price << " per cup" << endl;
        cout << endl;
    }
}

void fuelSalesStatistics(const Fuel fuels[FUEL_TYPES]) {

    double fuelLitersSold[FUEL_TYPES] = { 0 };
    double fuelSales[FUEL_TYPES] = { 0 };

    ifstream fuelSalesFile("fuelcache.txt");
    if (fuelSalesFile.is_open()) {
        string line;
        string currentFuelType = "";

        while (getline(fuelSalesFile, line)) 
        {
            if (line.find("Fuel type: ") != string::npos) 
            {
                currentFuelType = line.substr(line.find("Fuel type: ") + 11);
            }
            else if (line.find("Quantity sold: ") != string::npos) 
            {
                double litersSold = stod(line.substr(line.find("Quantity sold: ") + 15, line.find(" liters")));
                for (int i = 0; i < FUEL_TYPES; i++) 
                {
                    if (currentFuelType == fuels[i].type) 
                    {
                        fuelLitersSold[i] += litersSold;
                        break;
                    }
                }
            }
            else if (line.find("Total sales: $") != string::npos) 
            {
                double sales = stod(line.substr(line.find("Total sales: $") + 14));
                for (int i = 0; i < FUEL_TYPES; i++) 
                {
                    if (currentFuelType == fuels[i].type) 
                    {
                        fuelSales[i] += sales;
                        break;
                    }
                }
            }
        }
        fuelSalesFile.close();

        cout << "Fuel Sales Statistics:" << endl;
        for (int i = 0; i < FUEL_TYPES; i++)
        {
            cout << "Fuel type: " << fuels[i].type << endl;
            cout << "Total Liters Sold: " << fuelLitersSold[i] << " liters" << endl;
            cout << "Total Sales: $" << fuelSales[i] << endl;
            cout << endl;
        }
    }
    else 
    {
        cout << "Fuel sales file not found." << endl;
    }
}

void fuelCar(Fuel fuels[FUEL_TYPES], int& money) {
    int fuelChoice;
    readFuelData(fuels);
    cout << "Select fuel type (1 - " << FUEL_TYPES << "): ";
    cin >> fuelChoice;
    if (fuelChoice >= 1 && fuelChoice <= FUEL_TYPES) 
    {
        double amount;
        double totalCost;
        int purchaseOption;
        cout << "Choose purchase option:\n" << "1. Purchase by liters\n" << "2. Purchase by amount\n";
        cin >> purchaseOption;

        if (purchaseOption == 1) 
        {
            cout << "Enter amount to fuel (in liters): ";
            cin >> amount;
            if (amount * fuels[fuelChoice - 1].price > money) 
            {
                cout << "Not enough money to complete the purchase." << endl;
                return;
            }
            money -= amount * fuels[fuelChoice - 1].price;
            totalCost = amount * fuels[fuelChoice - 1].price;
        }
        else if (purchaseOption == 2) 
        {
            cout << "Enter amount to spend: $";
            double amountToSpend;
            cin >> amountToSpend;
            if (amountToSpend > money) 
            {
                cout << "Not enough money to complete the purchase." << endl;
                return;
            }
            money -= amountToSpend;
            amount = amountToSpend / fuels[fuelChoice - 1].price;
            totalCost = amountToSpend;
            cout << "You bought " << amount << " liters." << endl;
        }
        else 
        {
            cout << "Invalid purchase option" << endl;
            return;
        }

        if (amount > 0 && amount <= fuels[fuelChoice - 1].quantity) 
        {
            cout << "Total cost: $" << totalCost << endl;
            fuels[fuelChoice - 1].quantity -= amount;

            ofstream fuelSalesFile("fuelcache.txt", ios::app);
            if (fuelSalesFile.is_open()) 
            {
                fuelSalesFile << "Fuel type: " << fuels[fuelChoice - 1].type << endl;
                fuelSalesFile << "Quantity sold: " << amount << " liters" << endl;
                fuelSalesFile << "Total sales: $" << totalCost << endl;
                fuelSalesFile << endl;
                fuelSalesFile.close();
            }
            else 
            {
                cout << "Failed to buy." << endl;
            }
        }
        else 
        {
            cout << "Invalid amount" << endl;
        }
    }
    else 
    {
        cout << "Invalid fuel selection" << endl;
    }
}

void orderCoffee(Coffee coffees[COFFEE_TYPES], int& money) {

    readCoffeeData(coffees);
    int coffeeSelection;
    cout << "Select coffee type (1 - " << COFFEE_TYPES << "): ";
    cin >> coffeeSelection;
    if (coffeeSelection >= 1 && coffeeSelection <= COFFEE_TYPES) 
    {
        int quantity;
        cout << "Enter quantity of cups to order: ";
        cin >> quantity;
        if (quantity > 0 && quantity <= coffees[coffeeSelection - 1].quantity) 
        {
            double totalCost = quantity * coffees[coffeeSelection - 1].price;

            if (totalCost > money) 
            {
                cout << "Not enough money to purchase." << endl;
                return;
            }

            money -= totalCost;
            cout << "Total cost: $" << totalCost << endl;
            coffees[coffeeSelection - 1].quantity -= quantity;

            ofstream coffeeSalesFile("coffeecache.txt", ios::app);
            if (coffeeSalesFile.is_open())
            {
                coffeeSalesFile << "Coffee type: " << coffees[coffeeSelection - 1].type << endl;
                coffeeSalesFile << "Quantity sold: " << quantity << " cups" << endl;
                coffeeSalesFile << "Total sales: $" << totalCost << endl;
                coffeeSalesFile << endl;
                coffeeSalesFile.close();
            }
            else 
            {
                cout << "Failed to update coffee cache." << endl;
            }
        }
        else 
        {
            cout << "Invalid quantity." << endl;
        }
    }
    else 
    {
        cout << "Invalid coffee selection." << endl;
    }
}

void displaySales(const Fuel fuels[FUEL_TYPES], const Coffee coffees[COFFEE_TYPES]) {

    double totalLitersSold = 0;
    double totalSales = 0;

    ifstream fuelSalesFile("fuelcache.txt");
    if (fuelSalesFile.is_open())
    {
        string line;

        while (getline(fuelSalesFile, line)) 
        {
            if (line.find("Quantity sold: ") != string::npos) 
            {
                int litersSold = stoi(line.substr(line.find("Quantity sold: ") + 15));
                totalLitersSold += litersSold;
            }
            else if (line.find("Total sales: $") != string::npos) 
            {
                double sales = stod(line.substr(line.find("Total sales: $") + 14));
                totalSales += sales;
            }
        }
        fuelSalesFile.close();

        cout << "Fuel Sales Statistics:" << endl;
        cout << "Total Liters Sold: " << totalLitersSold << " liters" << endl;
        cout << "Total Sales: $" << totalSales << endl;
    }
    else 
    {
        cout << "Fuel sales file not found." << endl;
    }


    int totalSales1 = 0;
    int totalCupsSold = 0;
    ifstream coffeeSalesFile("coffeecache.txt");
    if (coffeeSalesFile.is_open())
    {
        string line;
        while (getline(coffeeSalesFile, line)) 
        {
            if (line.find("Quantity sold: ") != string::npos) 
            {
                int cupsSold = stoi(line.substr(line.find("Quantity sold: ") + 15));
                totalCupsSold += cupsSold;
            }
            else if (line.find("Total sales: $") != string::npos)
            {
                double sales = stod(line.substr(line.find("Total sales: $") + 14));
                totalSales1 += sales;
            }
        }
        coffeeSalesFile.close();

        cout << "Coffee Sales Statistics:" << endl;
        cout << "Total Cups Sold: " << totalCupsSold << " cups" << endl;
        cout << "Total Sales: $" << totalSales1 << endl;
    }
    else 
    {
        cout << "error" << endl;
    }


   
    cout << "Total earnings: $" << (totalSales + totalSales1) << endl;

}

void clearFuelCache() {
    ofstream file("fuelcache.txt");
    file.close();
    cout << "Fuel cache cleared." << endl;
}

void clearCoffeeCache() {
    ofstream file("coffeecache.txt");
    file.close();
    cout << "Coffee cache cleared." << endl;
}



int main() {
    Fuel fuels[FUEL_TYPES] = { {"A-95", 45, 0}, {"A-92", 42, 0}, {"Diesel", 40, 0} };
    Coffee coffees[COFFEE_TYPES] = { {"Americano", 25, 0}, {"Latte", 30, 0}, {"Cappuccino", 29, 0} };

    int mode = -1;
    while (mode != 0) 
    {
        cout << "Enter mode: (1 - Admin, 2 - User, 0 - Exit): ";
        cin >> mode;

        switch (mode) {
        case 1: 
        {
            int choice;
            do 
            {
                cout << endl << "Gas Station admin" << endl << endl
                    << "1. Refill fuel" << endl
                    << "2. Read Fuel data" << endl
                    << "3. Change fuel price" << endl
                    << "4. Refill coffee" << endl
                    << "5. Read coffee data" << endl
                    << "6. Clear fuel cache" << endl
                    << "7. Clear coffee cache" << endl
                    << "8. Display fuel sales statistics" << endl 
                    << "9. Display earned money" << endl
                    << "0. Exit" << endl << "-------------------------" << endl << "Enter your choice: ";
                cin >> choice;

                switch (static_cast<Menu>(choice)) {
                case REFILL_FUEL:
                    refillFuel(fuels);
                    break;
                case READ_FUEL_DATA:
                    readFuelData(fuels);
                    break;
                case CHANGE_FUEL_PRICE:
                    changeFuelPrice(fuels);
                    break;
                case REFILL_COFFEE:
                    refillCoffee(coffees);
                    break;
                case READ_COFFEE_DATA:
                    readCoffeeData(coffees);
                    break;
                case CLEAR_FUEL_CACHE:
                    clearFuelCache();
                    break;
                case CLEAR_COFFEE_CACHE:
                    clearCoffeeCache();
                    break;
                case FUEL_SALES_STATISTICS:
                    fuelSalesStatistics(fuels);
                    break;
                case DISPLAY_SALES:
                    displaySales(fuels, coffees);
                    break;
                case QUIT:
                    break;
                default:
                    break;
                }
            } while (choice != QUIT);
            break;
        }
        case 2: 
        {
            int choice, money;
            cout << endl << "Gas Station user" << endl << "Enter your cache in $ " << endl;
            cin >> money;
            do 
            {
                do 
                {
                   cout << endl << "Gas Station user" << endl << endl 
                        << "1. Order coffee" << endl 
                        << "2. Fuel car" << endl 
                        << "0. Exit" << endl 
                        << "-------------------------" << endl 
                        << "Enter your choice: ";
                    cin >> choice;

                    switch (static_cast<Menu1>(choice)) {
                    case ORDER_COFFEE:
                        orderCoffee(coffees, money);
                        break;
                    case FUEL_CAR:
                        fuelCar(fuels, money);
                        break;
                    case QUIT1:
                        break;
                    default:
                        std::cout << "Error" << std::endl;
                        break;
                    }
                } while (choice != QUIT);
                break;
            } while (money > 0);
        }
        case 0:
            break;
        default:
            cout << "Invalid mode selection." << endl;
            break;
        }
    }

    return 0;
}