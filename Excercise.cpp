
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// ---------------------- Product ----------------------
class Product {
private:
    int id;
    const char* name;
    double price;

public:
    Product() : id(0), name(""), price(0.0) {}
    Product(int i, const char* n, double p) : id(i), name(n), price(p) {}

    int getId() const { return id; }
    const char* getName() const { return name; }
    double getPrice() const { return price; }
};

// ---------------------- Payment Strategy ----------------------
class PaymentMethod {
public:
    virtual void pay(double amount) = 0;
};

class CashPayment : public PaymentMethod {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " using Cash.\n";
    }
};

class CreditCardPayment : public PaymentMethod {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " using Credit/Debit Card.\n";
    }
};

class GCashPayment : public PaymentMethod {
public:
    void pay(double amount) override {
        cout << "Paid " << amount << " using GCash.\n";
    }
};

// ---------------------- Singleton Payment Processor ----------------------
class PaymentProcessor {
private:
    static PaymentProcessor* instance;
    PaymentProcessor() {}

public:
    static PaymentProcessor* getInstance() {
        if (instance == nullptr) instance = new PaymentProcessor();
        return instance;
    }

    void processPayment(PaymentMethod* method, double amount) {
        method->pay(amount);
    }
};

PaymentProcessor* PaymentProcessor::instance = nullptr;

// ---------------------- Shopping Cart ----------------------
class ShoppingCartItem {
public:
    Product product;
    int quantity;

    ShoppingCartItem() : quantity(0) {}
    ShoppingCartItem(Product p, int q) : product(p), quantity(q) {}
};

class ShoppingCart {
private:
    ShoppingCartItem items[10];
    int itemCount;

public:
    ShoppingCart() : itemCount(0) {}

    void addProduct(Product p) {
        for (int i = 0; i < itemCount; i++) {
            if (items[i].product.getId() == p.getId()) {
                items[i].quantity++;
                return;
            }
        }
        items[itemCount++] = ShoppingCartItem(p, 1);
    }

    void viewCart() {
        cout << "\nShopping Cart:\n";
        cout << "ID\tName\t\tPrice\tQty\n";
        for (int i = 0; i < itemCount; i++) {
            cout << items[i].product.getId() << "\t" << items[i].product.getName()
                 << "\t\t" << items[i].product.getPrice() << "\t" << items[i].quantity << endl;
        }
    }

    double getTotal() {
        double total = 0.0;
        for (int i = 0; i < itemCount; i++) {
            total += items[i].product.getPrice() * items[i].quantity;
        }
        return total;
    }

    int getItemCount() const { return itemCount; }
    ShoppingCartItem* getItems() { return items; }
};

// ---------------------- Logger ----------------------
void logOrder(int orderId, const char* method) {
    ofstream file("orders.txt", ios::app);
    if (file.is_open()) {
        file << "[LOG] -> Order ID: " << orderId << " has been successfully checked out and paid using " << method << ".\n";
        file.close();
    }
}

// ---------------------- Main Program ----------------------
int main() {
    Product products[5] = {
        Product(1, "Keyboard", 899.0),
        Product(2, "Mouse", 599.0),
        Product(3, "Monitor", 5999.0),
        Product(4, "USB Cable", 199.0),
        Product(5, "Webcam", 1499.0)
    };

    ShoppingCart cart;
    int choice, id, orderId = 1001;
    char cont;

    do {
        cout << "\nMENU:\n1. View Products\n2. View Shopping Cart\n3. Exit\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\nProducts:\nID\tName\t\tPrice\n";
            for (int i = 0; i < 5; i++) {
                cout << products[i].getId() << "\t" << products[i].getName() << "\t\t" << products[i].getPrice() << endl;
            }
            do {
                cout << "Enter the ID of the product to add to cart: ";
                cin >> id;
                bool found = false;
                for (int i = 0; i < 5; i++) {
                    if (products[i].getId() == id) {
                        cart.addProduct(products[i]);
                        cout << "Product added successfully!\n";
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Invalid Product ID!\n";

                cout << "Do you want to add another product? (Y/N): ";
                cin >> cont;
            } while (cont == 'Y' || cont == 'y');

        } else if (choice == 2) {
            if (cart.getItemCount() == 0) {
                cout << "\nCart is empty!\n";
                continue;
            }
            cart.viewCart();
            cout << "\nDo you want to check out all the products? (Y/N): ";
            cin >> cont;
            if (cont == 'Y' || cont == 'y') {
                double total = cart.getTotal();
                cout << "Total Amount: " << total << endl;

                cout << "Select Payment Method: 1. Cash 2. Credit/Debit Card 3. GCash\nChoice: ";
                int pm;
                cin >> pm;

                PaymentMethod* method = nullptr;
                const char* methodStr = "";
                switch (pm) {
                    case 1: method = new CashPayment(); methodStr = "Cash"; break;
                    case 2: method = new CreditCardPayment(); methodStr = "Credit/Debit Card"; break;
                    case 3: method = new GCashPayment(); methodStr = "GCash"; break;
                    default: cout << "Invalid choice!"; continue;
                }

                PaymentProcessor::getInstance()->processPayment(method, total);
                logOrder(orderId++, methodStr);
                cout << "You have successfully checked out the products!\n";
                delete method;
            }
        }
    } while (choice != 3);

    return 0;
}
