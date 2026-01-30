#include "User.h"
#include <iostream>
#include <sstream>
#include <ctime>

using namespace std;

User::User() : id(0), role(UserRole::CLIENT), membership(nullptr) {
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    createdAt = buffer;
}

User::User(int id, const string& name, const string& login,
    const string& password, UserRole role)
    : id(id), name(name), login(login), password(password),
    role(role), membership(nullptr) {

    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    createdAt = buffer;
}

User::~User() {
    delete membership;
}

User::User(const User& other)
    : id(other.id), name(other.name), login(other.login), password(other.password),
    role(other.role), createdAt(other.createdAt) {

    if (other.membership) {
        membership = new Membership(*other.membership);
    }
    else {
        membership = nullptr;
    }
}

User& User::operator=(const User& other) {
    if (this != &other) {
        delete membership;

        id = other.id;
        name = other.name;
        login = other.login;
        password = other.password;
        role = other.role;
        createdAt = other.createdAt;

        if (other.membership) {
            membership = new Membership(*other.membership);
        }
        else {
            membership = nullptr;
        }
    }
    return *this;
}

// Геттери
int User::getId() const { return id; }
string User::getName() const { return name; }
string User::getLogin() const { return login; }
string User::getPassword() const { return password; }
UserRole User::getRole() const { return role; }
Membership* User::getMembership() const { return membership; }
string User::getCreatedAt() const { return createdAt; }

// Сеттери
void User::setName(const string& name) { this->name = name; }
void User::setLogin(const string& login) { this->login = login; }
void User::setPassword(const string& password) { this->password = password; }
void User::setRole(UserRole role) { this->role = role; }
void User::setMembership(Membership* membership) {
    delete this->membership;
    this->membership = membership;
}

// Операції з абонементом
void User::purchaseMembership(const string& name, int visits) {
    delete membership;
    membership = new Membership(name, visits);
}

void User::useVisit() {
    if (membership) {
        membership->useVisit();
    }
}

void User::freezeMembership(const string& untilDate) {
    if (membership) {
        membership->freeze(untilDate);
    }
}

void User::unfreezeMembership() {
    if (membership) {
        membership->unfreeze();
    }
}

void User::resetMembership() {
    if (membership) {
        membership->reset();
    }
}

void User::displayInfo() const {
    cout << "\n=== Інформація про користувача ===\n";
    cout << "ID: " << id << endl;
    cout << "Ім'я: " << name << endl;
    cout << "Логін: " << login << endl;
    cout << "Роль: " << roleToString(role) << endl;
    cout << "Дата реєстрації: " << createdAt << endl;

    if (membership) {
        membership->displayInfo();
    }
    else {
        cout << "Абонемент: відсутній\n";
    }
}

string User::toString() const {
    stringstream ss;
    ss << id << "|" << name << "|" << login << "|"
        << password << "|" << static_cast<int>(role) << "|"
        << createdAt << "|";

    if (membership) {
        ss << membership->toString();
    }
    else {
        ss << "NO_MEMBERSHIP";
    }

    return ss.str();
}

User User::fromString(const string& str) {
    User user;
    stringstream ss(str);
    string token;

    getline(ss, token, '|');
    user.id = stoi(token);

    getline(ss, token, '|');
    user.name = token;

    getline(ss, token, '|');
    user.login = token;

    getline(ss, token, '|');
    user.password = token;

    getline(ss, token, '|');
    user.role = static_cast<UserRole>(stoi(token));

    getline(ss, token, '|');
    user.createdAt = token;

    getline(ss, token, '|');
    if (token != "NO_MEMBERSHIP") {
        user.membership = new Membership(Membership::fromString(token));
    }

    return user;
}

bool User::hasMembership() const {
    return membership != nullptr;
}