#include "AuthSystem.h"
#include <iostream>

using namespace std;

AuthSystem::AuthSystem(Database* db) : database(db), currentUser(nullptr) {}

AuthSystem::~AuthSystem() {
    logout();
}

bool AuthSystem::login(const string& login, const string& password) {
    User* user = database->getUserByLogin(login);
    if (user && user->getPassword() == password) {
        currentUser = user;
        cout << "Вхід успішний! Вітаємо, " << user->getName() << "!\n";
        return true;
    }

    cout << "Невірний логін або пароль!\n";
    return false;
}

void AuthSystem::logout() {
    if (currentUser) {
        cout << "До побачення, " << currentUser->getName() << "!\n";
        currentUser = nullptr;
    }
}

bool AuthSystem::registerUser(const string& name, const string& login,
    const string& password, UserRole role) {
    int id = database->getNextId();
    User newUser(id, name, login, password, role);

    if (database->addUser(newUser)) {
        cout << "Реєстрація успішна!\n";
        return true;
    }

    cout << "Користувач з таким логіном вже існує!\n";
    return false;
}

User* AuthSystem::getCurrentUser() const {
    return currentUser;
}

bool AuthSystem::isLoggedIn() const {
    return currentUser != nullptr;
}

bool AuthSystem::hasPermission(UserRole requiredRole) const {
    if (!currentUser) return false;

    // HELPER має повний доступ
    if (currentUser->getRole() == UserRole::HELPER) return true;

    // ADMIN має доступ до адмін функцій
    if (requiredRole == UserRole::ADMIN) {
        return currentUser->getRole() == UserRole::ADMIN ||
            currentUser->getRole() == UserRole::HELPER;
    }

    // CLIENT має доступ тільки до своїх функцій
    if (requiredRole == UserRole::CLIENT) {
        return currentUser->getRole() == UserRole::CLIENT;
    }

    return false;
}

void AuthSystem::changePassword(const string& oldPassword, const string& newPassword) {
    if (!currentUser) {
        cout << "Спочатку увійдіть в систему!\n";
        return;
    }

    if (currentUser->getPassword() != oldPassword) {
        cout << "Невірний старий пароль!\n";
        return;
    }

    currentUser->setPassword(newPassword);
    database->updateUser(*currentUser);
    cout << "Пароль успішно змінено!\n";
}