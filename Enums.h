#ifndef ENUMS_H
#define ENUMS_H

#include <string>

enum class UserRole {
    CLIENT,
    ADMIN,
    HELPER
};

enum class MembershipStatus {
    ACTIVE,
    FROZEN,
    RESET,
    EXPIRED
};

std::string roleToString(UserRole role);
UserRole stringToRole(const std::string& roleStr);
std::string statusToString(MembershipStatus status);

#endif