#include "Database.h"
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

Database::Database(const string& filename) : filename(filename), nextId(1) {
    // Перевіряємо, чи існує файл
    ifstream testFile(filename);
    if (testFile.good()) {
        loadFromFile();
        // Знаходимо максимальний ID
        for (const auto& user : users) {
            if (user.getId() >= nextId) {
                nextId = user.getId() + 1;
            }
        }
    }
    testFile.close();
}

Database::~Database() {
    saveToFile();
}

void Database::loadFromFile() {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Помилка відкриття файлу: " << filename << endl;
        return;
    }

    users.clear();
    string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            try {
                users.push_back(User::fromString(line));
            }
            catch (const exception& e) {
                cerr << "Помилка парсингу рядка: " << e.what() << endl;
            }
        }
    }

    file.close();
}

void Database::saveToFile() {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Помилка відкриття файлу для запису: " << filename << endl;
        return;
    }

    for (const auto& user : users) {
        file << user.toString() << endl;
    }

    file.close();
}

bool Database::addUser(const User& user) {
    // Перевірка на унікальність логіну
    for (const auto& existingUser : users) {
        if (existingUser.getLogin() == user.getLogin()) {
            return false;
        }
    }

    users.push_back(user);
    saveToFile();
    return true;
}

bool Database::removeUser(int id) {
    auto it = remove_if(users.begin(), users.end(),
        [id](const User& user) { return user.getId() == id; });

    if (it != users.end()) {
        users.erase(it, users.end());
        saveToFile();
        return true;
    }

    return false;
}

bool Database::updateUser(const User& user) {
    for (auto& existingUser : users) {
        if (existingUser.getId() == user.getId()) {
            existingUser = user;
            saveToFile();
            return true;
        }
    }

    return false;
}

User* Database::getUser(int id) {
    for (auto& user : users) {
        if (user.getId() == id) {
            return &user;
        }
    }
    return nullptr;
}

User* Database::getUserByLogin(const string& login) {
    for (auto& user : users) {
        if (user.getLogin() == login) {
            return &user;
        }
    }
    return nullptr;
}

vector<User*> Database::getAllUsers() {
    vector<User*> result;
    for (auto& user : users) {
        result.push_back(&user);
    }
    return result;
}

vector<User*> Database::getUsersByRole(UserRole role) {
    vector<User*> result;
    for (auto& user : users) {
        if (user.getRole() == role) {
            result.push_back(&user);
        }
    }
    return result;
}

vector<User*> Database::searchUsersByName(const string& name) {
    vector<User*> result;
    string lowerName = name;
    transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    for (auto& user : users) {
        string userName = user.getName();
        transform(userName.begin(), userName.end(), userName.begin(), ::tolower);

        if (userName.find(lowerName) != string::npos) {
            result.push_back(&user);
        }
    }
    return result;
}

int Database::getUserCount() const {
    return static_cast<int>(users.size());
}

int Database::getUsersWithMembershipCount() const {
    int count = 0;
    for (const auto& user : users) {
        if (user.hasMembership()) {
            count++;
        }
    }
    return count;
}

void Database::displayAllUsers() const {
    cout << "\n=== Всі користувачі ===\n";
    for (const auto& user : users) {
        cout << "ID: " << user.getId()
            << ", Ім'я: " << user.getName()
            << ", Роль: " << roleToString(user.getRole()) << endl;
    }
    cout << "========================\n";
}

int Database::getNextId() {
    return nextId++;
}