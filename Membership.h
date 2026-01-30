#ifndef MEMBERSHIP_H
#define MEMBERSHIP_H

#include <string>
#include "Enums.h"

class Membership {
private:
    std::string name;
    int totalVisits;
    int remainingVisits;
    MembershipStatus status;
    std::string purchaseDate;
    std::string expirationDate;
    bool isFrozen;
    std::string frozenUntil;

public:
    Membership();
    Membership(const std::string& name, int totalVisits);

    void useVisit();
    void freeze(const std::string& untilDate);
    void unfreeze();
    void reset();
    void extend(int days);
    bool isValid() const;

    // Геттери
    std::string getName() const;
    int getTotalVisits() const;
    int getRemainingVisits() const;
    MembershipStatus getStatus() const;
    std::string getPurchaseDate() const;
    std::string getExpirationDate() const;
    bool getIsFrozen() const;
    std::string getFrozenUntil() const;

    // Сеттери
    void setName(const std::string& name);
    void setTotalVisits(int visits);
    void setRemainingVisits(int visits);
    void setStatus(MembershipStatus status);

    void displayInfo() const;
    std::string toString() const;
    static Membership fromString(const std::string& str);
};

#endif