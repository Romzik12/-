#ifndef USER_H
#define USER_H

#include <string>
#include "Membership.h"
#include "Enums.h"

class User {
private:
    int id;
    std::string name;
    std::string login;
    std::string password;
    UserRole role;
    Membership* membership;
    std::string createdAt;

public:
    User();
    User(int id, const std::string& name, const std::string& login,
        const std::string& password, UserRole role = UserRole::CLIENT);
    ~User();

    // Копіювання та присвоєння
    User(const User& other);
    User& operator=(const User& other);

    // Геттери
    int getId() const;
    std::string getName() const;
    std::string getLogin() const;
    std::string getPassword() const;
    UserRole getRole() const;
    Membership* getMembership() const;
    std::string getCreatedAt() const;

    // Сеттери
    void setName(const std::string& name);
    void setLogin(const std::string& login);
    void setPassword(const std::string& password);
    void setRole(UserRole role);
    void setMembership(Membership* membership);

    // Операції з абонементом
    void purchaseMembership(const std::string& name, int visits);
    void useVisit();
    void freezeMembership(const std::string& untilDate);
    void unfreezeMembership();
    void resetMembership();

    void displayInfo() const;
    std::string toString() const;
    static User fromString(const std::string& str);

    bool hasMembership() const;
};

#endif