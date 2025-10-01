/*
Name: Nguyễn Huỳnh Minh Tuyết
ID: 24110144
Project: Ecommerce Store Management System
*/

#include <iostream>
#include <vector>
#include <string>
using namespace std;

// -------------------- Product --------------------
class Product {
protected:
    string productID;
    string productName;
    double price;
    int stock; // số lượng còn trong kho

public:
    Product(string id, string name, double p, int s) {
        productID = id;
        productName = name;
        price = p;
        stock = s;
    }

    void setProductID(string id) { productID = id; }
    void setProductName(string name) { productName = name; }
    void setPrice(double p) { price = p; }
    void setStock(int s) { stock = s; }

    string getProductID() const { return productID; }
    string getProductName() const { return productName; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }

    void reduceStock(int qty) {
        if (qty <= stock) stock -= qty;
        else cout << "Not enough stock for " << productName << endl;
    }

    // in product
    friend ostream& operator<<(ostream& os, const Product& p) {
        os << p.productID << " - " << p.productName 
           << " (" << p.price << "$, stock: " << p.stock << ")";
        return os;
    }
};

// -------------------- CartItem --------------------
class CartItem {
protected:
    Product product;
    int quantity;
    double subtotal;

public:
    CartItem(Product p, int qty) : product(p), quantity(qty) {
        subtotal = p.getPrice() * qty;
    }

    void setProduct(const Product& p) { product = p; }
    void setQuantity(int q) { 
        quantity = q; 
        subtotal = product.getPrice() * q; 
    }

    int getQuantity() const { return quantity; }
    Product getProduct() const { return product; }
    double getSubtotal() const { return subtotal; }

    // operator += : tăng số lượng sản phẩm trong cart item
    CartItem& operator+=(int extraQty) {
        quantity += extraQty;
        subtotal = product.getPrice() * quantity;
        return *this;
    }

    // in cart item
    friend ostream& operator<<(ostream& os, const CartItem& ci) {
        os << ci.getProduct().getProductName() 
           << " x " << ci.quantity 
           << " = " << ci.subtotal << "$";
        return os;
    }
};

// -------------------- Cart --------------------
class Cart {
protected:
    string cartID;
    vector<CartItem> items;
    double totalAmount;

public:
    Cart(string id) {
        cartID = id;
        totalAmount = 0;
    }

    void setCartID(string id) { cartID = id; }
    void setTotalAmount(double amt) { totalAmount = amt; }
    string getCartID() const { return cartID; }
    double getTotalAmount() const { return totalAmount; }

    // operator += : thêm sản phẩm vào giỏ
    Cart& operator+=(const CartItem& item) {
        items.push_back(item);
        calculateTotal();
        return *this;
    }

    // operator == : so sánh tổng tiền giữa 2 cart
    bool operator==(const Cart& other) const {
        return this->totalAmount == other.totalAmount;
    }

    void calculateTotal() {
        totalAmount = 0;
        for (const auto& it : items) {
            totalAmount += it.getSubtotal();
        }
    }

    void displayCart() const {
        cout << "Cart ID: " << cartID << endl;
        for (const auto& it : items) {
            cout << "  - " << it << endl;
        }
        cout << "Total: " << totalAmount << "$" << endl;
    }
};

// -------------------- Customer --------------------
class Customer {
protected:
    string name;
    string customerID;
    Cart cart;

public:
    Customer(string n, string id) : name(n), customerID(id), cart("CART-" + id) {}

    void setName(string n) { name = n; }
    void setCustomerID(string id) { customerID = id; }

    string getName() const { return name; }
    string getCustomerID() const { return customerID; }

    Cart& getCart() { return cart; }              // non-const version
    const Cart& getCart() const { return cart; }  // const version                                                                                                                                                                                      
};


// -------------------- Operations --------------------
class Operations {
protected:
    vector<Product> products;
    vector<Customer> customers;

public:
    void addProduct(const Product& p) {
        products.push_back(p);
    }

    void addCustomer(const Customer& c) {
        customers.push_back(c);
    }

    void displayProducts() const {
        cout << "Available Products:" << endl;
        for (const auto& p : products) {
            cout << "  - " << p << endl;
        }
    }

    // thực hiện thêm vào giỏ
    void addToCart(const string& custID, const string& prodID, int qty) {
        for (auto& cust : customers) {
            if (cust.getCustomerID() == custID) {
                for (auto& p : products) {
                    if (p.getProductID() == prodID) {
                        if (qty <= p.getStock()) {
                            p.reduceStock(qty);
                            CartItem ci(p, qty);
                            cust.getCart() += ci; // dùng operator +=
                            cout << "Added to cart: " << p.getProductName() << " x" << qty << endl;
                        } else {
                            cout << "Not enough stock!" << endl;
                        }
                        return;
                    }
                }
            }
        }
    }

    void displayCustomerCart(const string& custID) const {
       for (auto& cust : customers) {

            if (cust.getCustomerID() == custID) {
                cout << "Customer: " << cust.getName() << endl;
                cust.getCart().displayCart();
            }
        }
    }
};

// -------------------- MAIN --------------------
int main() {
    Operations ops;

    // Tạo sản phẩm
    Product p1("P001", "T-Shirt", 15.0, 50);
    Product p2("P002", "Jeans", 40.0, 30);
    Product p3("P003", "Sneakers", 60.0, 20);

    ops.addProduct(p1);
    ops.addProduct(p2);
    ops.addProduct(p3);

    // Tạo khách hàng
    Customer c1("Nguyen Van ", "C001");
    Customer c2("Le Thanh", "C002");

    ops.addCustomer(c1);
    ops.addCustomer(c2);

    // Hiển thị sản phẩm
    ops.displayProducts();

    // Thêm hàng vào giỏ
    ops.addToCart("C001", "P001", 2);
    ops.addToCart("C001", "P003", 1);
    ops.addToCart("C002", "P002", 3);

    // Hiển thị giỏ hàng
    ops.displayCustomerCart("C001");
    ops.displayCustomerCart("C002");

    return 0;
}

