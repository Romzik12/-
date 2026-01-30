#include "Membership.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>

using namespace std;

Membership::Membership()
    : name(""), totalVisits(0), remainingVisits(0), status(MembershipStatus::ACTIVE),
    isFrozen(false) {

    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    purchaseDate = buffer;

    // Додаємо 30 днів до терміну дії
    localTime.tm_mday += 30;
    mktime(&localTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    expirationDate = buffer;
}

Membership::Membership(const string& name, int totalVisits)
    : name(name), totalVisits(totalVisits), remainingVisits(totalVisits),
    status(MembershipStatus::ACTIVE), isFrozen(false) {

    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    purchaseDate = buffer;

    // Додаємо 30 днів до терміну дії
    localTime.tm_mday += 30;
    mktime(&localTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    expirationDate = buffer;
}

void Membership::useVisit() {
    if (isValid() && remainingVisits > 0) {
        remainingVisits--;
        if (remainingVisits == 0) {
            status = MembershipStatus::EXPIRED;
        }
    }
}

void Membership::freeze(const string& untilDate) {
    isFrozen = true;
    frozenUntil = untilDate;
    status = MembershipStatus::FROZEN;
}

void Membership::unfreeze() {
    isFrozen = false;
    frozenUntil = "";
    status = MembershipStatus::ACTIVE;
}

void Membership::reset() {
    remainingVisits = totalVisits;
    status = MembershipStatus::ACTIVE;
    isFrozen = false;
    frozenUntil = "";
}

void Membership::extend(int days) {
    // Проста реалізація продовження
    time_t now = time(nullptr);
    tm localTime;

    // Розбираємо поточну дату закінчення
    int year, month, day;
    sscanf_s(expirationDate.c_str(), "%d-%d-%d", &year, &month, &day);

    localTime.tm_year = year - 1900;
    localTime.tm_mon = month - 1;
    localTime.tm_mday = day;
    localTime.tm_hour = 0;
    localTime.tm_min = 0;
    localTime.tm_sec = 0;
    localTime.tm_isdst = -1;

    // Додаємо дні
    localTime.tm_mday += days;
    mktime(&localTime);

    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    expirationDate = buffer;
}

bool Membership::isValid() const {
    if (status == MembershipStatus::EXPIRED || status == MembershipStatus::RESET) {
        return false;
    }

    // Перевірка терміну дії
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);

    int currentYear = localTime.tm_year + 1900;
    int currentMonth = localTime.tm_mon + 1;
    int currentDay = localTime.tm_mday;

    int expYear, expMonth, expDay;
    sscanf_s(expirationDate.c_str(), "%d-%d-%d", &expYear, &expMonth, &expDay);

    if (currentYear > expYear) return false;
    if (currentYear == expYear && currentMonth > expMonth) return false;
    if (currentYear == expYear && currentMonth == expMonth && currentDay > expDay) return false;

    return true;
}

// Геттери
string Membership::getName() const { return name; }
int Membership::getTotalVisits() const { return totalVisits; }
int Membership::getRemainingVisits() const { return remainingVisits; }
MembershipStatus Membership::getStatus() const { return status; }
string Membership::getPurchaseDate() const { return purchaseDate; }
string Membership::getExpirationDate() const { return expirationDate; }
bool Membership::getIsFrozen() const { return isFrozen; }
string Membership::getFrozenUntil() const { return frozenUntil; }

// Сеттери
void Membership::setName(const string& name) { this->name = name; }
void Membership::setTotalVisits(int visits) { totalVisits = visits; }
void Membership::setRemainingVisits(int visits) { remainingVisits = visits; }
void Membership::setStatus(MembershipStatus status) { this->status = status; }

void Membership::displayInfo() const {
    cout << "\n=== Інформація про абонемент ===\n";
    cout << "Назва: " << name << endl;
    cout << "Загальна кількість відвідувань: " << totalVisits << endl;
    cout << "Залишилось відвідувань: " << remainingVisits << endl;
    cout << "Статус: " << statusToString(status) << endl;
    cout << "Дата покупки: " << purchaseDate << endl;
    cout << "Термін дії до: " << expirationDate << endl;
    if (isFrozen) {
        cout << "Заморожено до: " << frozenUntil << endl;
    }
    cout << "Дійсний: " << (isValid() ? "Так" : "Ні") << endl;
}

string Membership::toString() const {
    stringstream ss;
    ss << name << "|" << totalVisits << "|" << remainingVisits << "|"
        << static_cast<int>(status) << "|" << purchaseDate << "|"
        << expirationDate << "|" << (isFrozen ? "1" : "0") << "|" << frozenUntil;
    return ss.str();
}

Membership Membership::fromString(const string& str) {
    Membership membership;
    stringstream ss(str);
    string token;

    getline(ss, token, '|');
    membership.name = token;

    getline(ss, token, '|');
    membership.totalVisits = stoi(token);

    getline(ss, token, '|');
    membership.remainingVisits = stoi(token);

    getline(ss, token, '|');
    membership.status = static_cast<MembershipStatus>(stoi(token));

    getline(ss, token, '|');
    membership.purchaseDate = token;

    getline(ss, token, '|');
    membership.expirationDate = token;

    getline(ss, token, '|');
    membership.isFrozen = (token == "1");

    getline(ss, token);
    membership.frozenUntil = token;

    return membership;
}