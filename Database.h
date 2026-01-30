#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <fstream>
#include "User.h"

class Database {
private:
    std::vector<User> users;
    std::string filename;
    int nextId;

    void loadFromFile();
    void saveToFile();

public:
    Database(const std::string& filename);
    ~Database();

    // Основний функціонал
    bool addUser(const User& user);
    bool removeUser(int id);
    bool updateUser(const User& user);
    User* getUser(int id);
    User* getUserByLogin(const std::string& login);

    // Пошук та фільтрація
    std::vector<User*> getAllUsers();
    std::vector<User*> getUsersByRole(UserRole role);
    std::vector<User*> searchUsersByName(const std::string& name);

    // Статистика
    int getUserCount() const;
    int getUsersWithMembershipCount() const;

    void displayAllUsers() const;

    int getNextId();
};

#endif