#include "Enums.h"
#include <map>

std::string roleToString(UserRole role) {
    static std::map<UserRole, std::string> roleMap = {
        {UserRole::CLIENT, "CLIENT"},
        {UserRole::ADMIN, "ADMIN"},
        {UserRole::HELPER, "HELPER"}
    };
    return roleMap[role];
}

UserRole stringToRole(const std::string& roleStr) {
    static std::map<std::string, UserRole> stringMap = {
        {"CLIENT", UserRole::CLIENT},
        {"ADMIN", UserRole::ADMIN},
        {"HELPER", UserRole::HELPER}
    };
    auto it = stringMap.find(roleStr);
    return (it != stringMap.end()) ? it->second : UserRole::CLIENT;
}

std::string statusToString(MembershipStatus status) {
    static std::map<MembershipStatus, std::string> statusMap = {
        {MembershipStatus::ACTIVE, "ACTIVE"},
        {MembershipStatus::FROZEN, "FROZEN"},
        {MembershipStatus::RESET, "RESET"},
        {MembershipStatus::EXPIRED, "EXPIRED"}
    };
    return statusMap[status];
}