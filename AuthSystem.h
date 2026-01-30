#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <string>
#include "User.h"
#include "Database.h"

class AuthSystem {
private:
    Database* database;
    User* currentUser;

public:
    AuthSystem(Database* db);
    ~AuthSystem();

    bool login(const std::string& login, const std::string& password);
    void logout();
    bool registerUser(const std::string& name, const std::string& login,
        const std::string& password, UserRole role = UserRole::CLIENT);

    User* getCurrentUser() const;
    bool isLoggedIn() const;
    bool hasPermission(UserRole requiredRole) const;

    void changePassword(const std::string& oldPassword, const std::string& newPassword);
};

#endif