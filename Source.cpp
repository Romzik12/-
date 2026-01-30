#include <iostream>
#include <vector>
#include <string>

using namespace std;

// ===== Клас User =====
class User {
private:
    int id;
    string name;

public:
    User(int userId, const string& userName) {
        id = userId;
        name = userName;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    void setName(const string& newName) {
        name = newName;
    }

    void show() const {
        cout << "ID: " << id << ", Name: " << name << endl;
    }
};

// ===== Клас Admin =====
class Admin {
private:
    string password;

public:
    Admin(const string& pass) {
        password = pass;
    }

    // Перевірка пароля
    bool login(const string& inputPassword) const {
        return inputPassword == password;
    }

    // Змінити ім'я користувача (тільки після логіну)
    void changeUserName(vector<User>& users, int userId, const string& newName) {
        for (User& u : users) {
            if (u.getId() == userId) {
                u.setName(newName);
                cout << "Name changed successfully!\n";
                return;
            }
        }
        cout << "User not found!\n";
    }

    // Видалити користувача
    void deleteUser(vector<User>& users, int userId) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->getId() == userId) {
                users.erase(it);
                cout << "User deleted successfully!\n";
                return;
            }
        }
        cout << "User not found!\n";
    }
};

int main() {
    vector<User> users;
    users.emplace_back(1, "Chip Yuriy");
    users.emplace_back(2, "Meduliak Roman");
    users.emplace_back(3, "Popuk Oleg");

    Admin admin("admin123"); // пароль адміна
    string inputPassword;

    cout << "Enter admin password: ";
    cin >> inputPassword;

    if (!admin.login(inputPassword)) {
        cout << "Access denied! You are not admin.\n";
        return 0;
    }

    cout << "Access granted!\n";

    int choice;
    do {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. Show users\n";
        cout << "2. Change user name\n";
        cout << "3. Delete user\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            for (const User& u : users) {
                u.show();
            }
        }
        else if (choice == 2) {
            int id;
            string newName;
            cout << "Enter user ID: ";
            cin >> id;
            cout << "Enter new name: ";
            cin >> newName;
            admin.changeUserName(users, id, newName);
        }
        else if (choice == 3) {
            int id;
            cout << "Enter user ID: ";
            cin >> id;
            admin.deleteUser(users, id);
        }

    } while (choice != 0);

    return 0;
}
