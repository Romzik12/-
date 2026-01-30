#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <Windows.h>
#include "Database.h"
#include "AuthSystem.h"
#include "User.h"

using namespace std;

// Функції для різних ролей
void clientMenu(Database& db, AuthSystem& auth);
void adminMenu(Database& db, AuthSystem& auth);
void helperMenu(Database& db, AuthSystem& auth);
void displayMainMenu();

// Допоміжні функції
void clearScreen() {
    system("cls");
}

void pressEnterToContinue() {
    cout << "\nНатисніть Enter для продовження...";
    cin.ignore((numeric_limits<streamsize>::max)(), '\n');
    cin.get();
}

// Функція для створення тестових користувачів
void createTestUsers(Database& db) {
    cout << "Створення тестових користувачів...\n";

    // Створюємо користувачів напряму через Database
    User admin(1, "Адміністратор", "admin", "admin123", UserRole::ADMIN);
    User helper(2, "Помічник", "helper", "helper123", UserRole::HELPER);
    User client1(3, "Іван Петренко", "ivan", "ivan123", UserRole::CLIENT);
    User client2(4, "Марія Коваленко", "maria", "maria123", UserRole::CLIENT);
    User client3(5, "Олександр Сидоренко", "alex", "alex123", UserRole::CLIENT);

    // Додаємо абонементи
    client1.purchaseMembership("Стандартний", 12);
    client2.purchaseMembership("Преміум", 30);

    // Додаємо користувачів у базу
    db.addUser(admin);
    db.addUser(helper);
    db.addUser(client1);
    db.addUser(client2);
    db.addUser(client3);

    cout << "Тестові користувачі створені успішно!\n";
    cout << "\n=== Тестові облікові записи ===\n";
    cout << "Адміністратор:\n";
    cout << "  Логін: admin\n";
    cout << "  Пароль: admin123\n\n";
    cout << "Помічник:\n";
    cout << "  Логін: helper\n";
    cout << "  Пароль: helper123\n\n";
    cout << "Клієнт 1:\n";
    cout << "  Логін: ivan\n";
    cout << "  Пароль: ivan123\n\n";
    cout << "Клієнт 2:\n";
    cout << "  Логін: maria\n";
    cout << "  Пароль: maria123\n\n";
    cout << "Клієнт 3:\n";
    cout << "  Логін: alex\n";
    cout << "  Пароль: alex123\n";
    cout << "==============================\n";
}

// Функції для роботи з меню
void clientMenu(Database& db, AuthSystem& auth) {
    User* currentUser = auth.getCurrentUser();
    bool inMenu = true;

    while (inMenu && auth.isLoggedIn()) {
        clearScreen();
        cout << "=== Меню клієнта ===\n\n";
        cout << "1. Переглянути мій абонемент\n";
        cout << "2. Використати відвідування\n";
        cout << "3. Купити новий абонемент\n";
        cout << "4. Заморозити абонемент\n";
        cout << "5. Розморозити абонемент\n";
        cout << "6. Обнулити абонемент\n";
        cout << "7. Змінити пароль\n";
        cout << "8. Вийти з акаунту\n";
        cout << "0. Вийти з програми\n\n";

        cout << "Виберіть опцію: ";
        int choice;
        cin >> choice;
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        switch (choice) {
        case 1:
            if (currentUser->hasMembership()) {
                currentUser->getMembership()->displayInfo();
            }
            else {
                cout << "У вас немає активного абонемента.\n";
            }
            pressEnterToContinue();
            break;

        case 2:
            if (currentUser->hasMembership()) {
                currentUser->useVisit();
                db.updateUser(*currentUser);
                cout << "Відвідування використано!\n";
                cout << "Залишилось відвідувань: "
                    << currentUser->getMembership()->getRemainingVisits() << endl;
            }
            else {
                cout << "У вас немає активного абонемента.\n";
            }
            pressEnterToContinue();
            break;

        case 3: {
            string name;
            int visits;
            cout << "Назва абонемента: ";
            getline(cin, name);
            cout << "Кількість відвідувань: ";
            cin >> visits;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            currentUser->purchaseMembership(name, visits);
            db.updateUser(*currentUser);
            cout << "Абонемент куплено успішно!\n";
            pressEnterToContinue();
            break;
        }

        case 4:
            if (currentUser->hasMembership()) {
                string untilDate;
                cout << "Заморозити до (рррр-мм-дд): ";
                getline(cin, untilDate);
                currentUser->freezeMembership(untilDate);
                db.updateUser(*currentUser);
                cout << "Абонемент заморожено!\n";
            }
            else {
                cout << "У вас немає активного абонемента.\n";
            }
            pressEnterToContinue();
            break;

        case 5:
            if (currentUser->hasMembership()) {
                currentUser->unfreezeMembership();
                db.updateUser(*currentUser);
                cout << "Абонемент розморожено!\n";
            }
            else {
                cout << "У вас немає активного абонемента.\n";
            }
            pressEnterToContinue();
            break;

        case 6:
            if (currentUser->hasMembership()) {
                currentUser->resetMembership();
                db.updateUser(*currentUser);
                cout << "Абонемент обнулено!\n";
            }
            else {
                cout << "У вас немає активного абонемента.\n";
            }
            pressEnterToContinue();
            break;

        case 7: {
            string oldPass, newPass;
            cout << "Старий пароль: ";
            getline(cin, oldPass);
            cout << "Новий пароль: ";
            getline(cin, newPass);
            auth.changePassword(oldPass, newPass);
            pressEnterToContinue();
            break;
        }

        case 8:
            auth.logout();
            inMenu = false;
            cout << "Ви вийшли з акаунту.\n";
            pressEnterToContinue();
            break;

        case 0:
            auth.logout();
            exit(0);
            break;

        default:
            cout << "Невірний вибір!\n";
            pressEnterToContinue();
        }
    }
}

void adminMenu(Database& db, AuthSystem& auth) {
    User* currentUser = auth.getCurrentUser();
    bool inMenu = true;

    while (inMenu && auth.isLoggedIn()) {
        clearScreen();
        cout << "=== Меню адміністратора ===\n\n";
        cout << "1. Переглянути всіх користувачів\n";
        cout << "2. Знайти користувача за іменем\n";
        cout << "3. Переглянути інформацію про користувача\n";
        cout << "4. Додати нового користувача\n";
        cout << "5. Видалити користувача\n";
        cout << "6. Переглянути статистику\n";
        cout << "7. Перевірити абонементи користувачів\n";
        cout << "8. Змінити пароль\n";
        cout << "9. Вийти з акаунту\n";
        cout << "0. Вийти з програми\n\n";

        cout << "Виберіть опцію: ";
        int choice;
        cin >> choice;
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        switch (choice) {
        case 1:
            db.displayAllUsers();
            pressEnterToContinue();
            break;

        case 2: {
            string name;
            cout << "Введіть ім'я для пошуку: ";
            getline(cin, name);
            vector<User*> users = db.searchUsersByName(name);

            if (users.empty()) {
                cout << "Користувачів не знайдено.\n";
            }
            else {
                cout << "\nЗнайдені користувачі:\n";
                for (const auto& user : users) {
                    cout << "ID: " << user->getId()
                        << ", Ім'я: " << user->getName()
                        << ", Роль: " << roleToString(user->getRole()) << endl;
                }
            }
            pressEnterToContinue();
            break;
        }

        case 3: {
            int id;
            cout << "Введіть ID користувача: ";
            cin >> id;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            User* user = db.getUser(id);
            if (user) {
                user->displayInfo();
            }
            else {
                cout << "Користувача з таким ID не знайдено.\n";
            }
            pressEnterToContinue();
            break;
        }

        case 4: {
            string name, login, password;
            int roleChoice;

            cout << "Повне ім'я: ";
            getline(cin, name);
            cout << "Логін: ";
            getline(cin, login);
            cout << "Пароль: ";
            getline(cin, password);

            cout << "Роль (1 - Клієнт, 2 - Адміністратор, 3 - Помічник): ";
            cin >> roleChoice;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            UserRole role = UserRole::CLIENT;
            switch (roleChoice) {
            case 1: role = UserRole::CLIENT; break;
            case 2: role = UserRole::ADMIN; break;
            case 3: role = UserRole::HELPER; break;
            }

            // Створюємо нового користувача
            int newId = db.getNextId();
            User newUser(newId, name, login, password, role);

            if (db.addUser(newUser)) {
                cout << "Користувача успішно додано!\n";
            }
            else {
                cout << "Помилка: користувач з таким логіном вже існує!\n";
            }
            pressEnterToContinue();
            break;
        }

        case 5: {
            int id;
            cout << "Введіть ID користувача для видалення: ";
            cin >> id;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            if (db.removeUser(id)) {
                cout << "Користувача успішно видалено!\n";
            }
            else {
                cout << "Користувача з таким ID не знайдено.\n";
            }
            pressEnterToContinue();
            break;
        }

        case 6:
            cout << "\n=== Статистика ===\n";
            cout << "Загальна кількість користувачів: " << db.getUserCount() << endl;
            cout << "Кількість клієнтів з абонементами: " << db.getUsersWithMembershipCount() << endl;

            // Додаткова статистика
            vector<User*> clients = db.getUsersByRole(UserRole::CLIENT);
            int activeMemberships = 0;
            int frozenMemberships = 0;

            for (const auto& client : clients) {
                if (client->hasMembership()) {
                    Membership* mem = client->getMembership();
                    if (mem->getStatus() == MembershipStatus::ACTIVE) {
                        activeMemberships++;
                    }
                    else if (mem->getStatus() == MembershipStatus::FROZEN) {
                        frozenMemberships++;
                    }
                }
            }

            cout << "Активних абонементів: " << activeMemberships << endl;
            cout << "Заморожених абонементів: " << frozenMemberships << endl;
            pressEnterToContinue();
            break;

        case 7: {
            vector<User*> allUsers = db.getAllUsers();
            cout << "\n=== Інформація про абонементи користувачів ===\n";

            bool found = false;
            for (const auto& user : allUsers) {
                if (user->hasMembership()) {
                    found = true;
                    cout << "\nКористувач: " << user->getName()
                        << " (ID: " << user->getId() << ")\n";
                    user->getMembership()->displayInfo();
                }
            }

            if (!found) {
                cout << "Абонементів не знайдено.\n";
            }
            pressEnterToContinue();
            break;
        }

        case 8: {
            string oldPass, newPass;
            cout << "Старий пароль: ";
            getline(cin, oldPass);
            cout << "Новий пароль: ";
            getline(cin, newPass);
            auth.changePassword(oldPass, newPass);
            pressEnterToContinue();
            break;
        }

        case 9:
            auth.logout();
            inMenu = false;
            cout << "Ви вийшли з акаунту.\n";
            pressEnterToContinue();
            break;

        case 0:
            auth.logout();
            exit(0);
            break;

        default:
            cout << "Невірний вибір!\n";
            pressEnterToContinue();
        }
    }
}

void helperMenu(Database& db, AuthSystem& auth) {
    User* currentUser = auth.getCurrentUser();
    bool inMenu = true;

    while (inMenu && auth.isLoggedIn()) {
        clearScreen();
        cout << "=== Меню помічника ===\n\n";
        cout << "1. Переглянути всіх користувачів\n";
        cout << "2. Знайти користувача за іменем\n";
        cout << "3. Переглянути інформацію про користувача\n";
        cout << "4. Додати нового користувача\n";
        cout << "5. Видалити користувача\n";
        cout << "6. Переглянути статистику\n";
        cout << "7. Перевірити абонементи користувачів\n";
        cout << "8. Змінити пароль\n";
        cout << "9. Вийти з акаунту\n";
        cout << "10. Перейти в меню адміністратора\n";
        cout << "11. Перейти в меню клієнта\n";
        cout << "0. Вийти з програми\n\n";

        cout << "Виберіть опцію: ";
        int choice;
        cin >> choice;
        cin.ignore((numeric_limits<streamsize>::max)(), '\n');

        switch (choice) {
        case 1:
            db.displayAllUsers();
            pressEnterToContinue();
            break;

        case 2: {
            string name;
            cout << "Введіть ім'я для пошуку: ";
            getline(cin, name);
            vector<User*> users = db.searchUsersByName(name);

            if (users.empty()) {
                cout << "Користувачів не знайдено.\n";
            }
            else {
                cout << "\nЗнайдені користувачі:\n";
                for (const auto& user : users) {
                    cout << "ID: " << user->getId()
                        << ", Ім'я: " << user->getName()
                        << ", Роль: " << roleToString(user->getRole()) << endl;
                }
            }
            pressEnterToContinue();
            break;
        }

        case 3: {
            int id;
            cout << "Введіть ID користувача: ";
            cin >> id;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            User* user = db.getUser(id);
            if (user) {
                user->displayInfo();
            }
            else {
                cout << "Користувача з таким ID не знайдено.\n";
            }
            pressEnterToContinue();
            break;
        }

        case 4: {
            string name, login, password;
            int roleChoice;

            cout << "Повне ім'я: ";
            getline(cin, name);
            cout << "Логін: ";
            getline(cin, login);
            cout << "Пароль: ";
            getline(cin, password);

            cout << "Роль (1 - Клієнт, 2 - Адміністратор, 3 - Помічник): ";
            cin >> roleChoice;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            UserRole role = UserRole::CLIENT;
            switch (roleChoice) {
            case 1: role = UserRole::CLIENT; break;
            case 2: role = UserRole::ADMIN; break;
            case 3: role = UserRole::HELPER; break;
            }

            // Створюємо нового користувача
            int newId = db.getNextId();
            User newUser(newId, name, login, password, role);

            if (db.addUser(newUser)) {
                cout << "Користувача успішно додано!\n";
            }
            else {
                cout << "Помилка: користувач з таким логіном вже існує!\n";
            }
            pressEnterToContinue();
            break;
        }

        case 5: {
            int id;
            cout << "Введіть ID користувача для видалення: ";
            cin >> id;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            if (db.removeUser(id)) {
                cout << "Користувача успішно видалено!\n";
            }
            else {
                cout << "Користувача з таким ID не знайдено.\n";
            }
            pressEnterToContinue();
            break;
        }

        case 6:
            cout << "\n=== Статистика ===\n";
            cout << "Загальна кількість користувачів: " << db.getUserCount() << endl;
            cout << "Кількість клієнтів з абонементами: " << db.getUsersWithMembershipCount() << endl;
            pressEnterToContinue();
            break;

        case 7: {
            vector<User*> allUsers = db.getAllUsers();
            cout << "\n=== Інформація про абонементи користувачів ===\n";

            for (const auto& user : allUsers) {
                if (user->hasMembership()) {
                    cout << "\nКористувач: " << user->getName()
                        << " (ID: " << user->getId() << ")\n";
                    user->getMembership()->displayInfo();
                }
            }
            pressEnterToContinue();
            break;
        }

        case 8: {
            string oldPass, newPass;
            cout << "Старий пароль: ";
            getline(cin, oldPass);
            cout << "Новий пароль: ";
            getline(cin, newPass);
            auth.changePassword(oldPass, newPass);
            pressEnterToContinue();
            break;
        }

        case 9:
            auth.logout();
            inMenu = false;
            cout << "Ви вийшли з акаунту.\n";
            pressEnterToContinue();
            break;

        case 10:
            // Перехід в меню адміністратора
            adminMenu(db, auth);
            break;

        case 11:
            // Перехід в меню клієнта
            clientMenu(db, auth);
            break;

        case 0:
            auth.logout();
            exit(0);
            break;

        default:
            cout << "Невірний вибір!\n";
            pressEnterToContinue();
        }
    }
}

void displayMainMenu() {
    cout << "1. Увійти в систему\n";
    cout << "2. Зареєструватися (тільки для клієнтів)\n";
    cout << "3. Показати тестові облікові записи\n";
    cout << "0. Вийти\n";
}

int main() {
    // Налаштування кодування для української мови
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Database db("users.dat");
    AuthSystem auth(&db);

    // Створення тестових користувачів, якщо база порожня
    if (db.getUserCount() == 0) {
        createTestUsers(db);
        pressEnterToContinue();
    }

    bool running = true;

    while (running) {
        clearScreen();
        cout << "=== Абонементна система спортзалу ===\n\n";

        if (auth.isLoggedIn()) {
            User* currentUser = auth.getCurrentUser();
            cout << "Вітаємо, " << currentUser->getName()
                << " (" << roleToString(currentUser->getRole()) << ")\n\n";

            switch (currentUser->getRole()) {
            case UserRole::CLIENT:
                clientMenu(db, auth);
                break;
            case UserRole::ADMIN:
                adminMenu(db, auth);
                break;
            case UserRole::HELPER:
                helperMenu(db, auth);
                break;
            }
        }
        else {
            displayMainMenu();

            int choice;
            cout << "Виберіть опцію: ";
            cin >> choice;
            cin.ignore((numeric_limits<streamsize>::max)(), '\n');

            switch (choice) {
            case 1: {
                string login, password;
                cout << "\n=== Вхід в систему ===\n";
                cout << "Логін: ";
                getline(cin, login);
                cout << "Пароль: ";
                getline(cin, password);

                if (auth.login(login, password)) {
                    cout << "Вхід успішний!\n";
                }
                else {
                    cout << "Невірний логін або пароль!\n";
                }
                pressEnterToContinue();
                break;
            }
            case 2: {
                string name, login, password;
                cout << "\n=== Реєстрація нового клієнта ===\n";
                cout << "Повне ім'я: ";
                getline(cin, name);
                cout << "Логін: ";
                getline(cin, login);
                cout << "Пароль: ";
                getline(cin, password);

                if (auth.registerUser(name, login, password, UserRole::CLIENT)) {
                    cout << "Реєстрація успішна! Тепер ви можете увійти в систему.\n";
                }
                else {
                    cout << "Помилка: користувач з таким логіном вже існує!\n";
                }
                pressEnterToContinue();
                break;
            }
            case 3: {
                // Показати тестові облікові записи
                clearScreen();
                cout << "=== Тестові облікові записи ===\n\n";
                cout << "Адміністратор:\n";
                cout << "  Логін: admin\n";
                cout << "  Пароль: admin123\n\n";
                cout << "Помічник:\n";
                cout << "  Логін: helper\n";
                cout << "  Пароль: helper123\n\n";
                cout << "Клієнт 1:\n";
                cout << "  Логін: ivan\n";
                cout << "  Пароль: ivan123\n";
                cout << "  (має абонемент 'Стандартний' на 12 відвідувань)\n\n";
                cout << "Клієнт 2:\n";
                cout << "  Логін: maria\n";
                cout << "  Пароль: maria123\n";
                cout << "  (має абонемент 'Преміум' на 30 відвідувань)\n\n";
                cout << "Клієнт 3:\n";
                cout << "  Логін: alex\n";
                cout << "  Пароль: alex123\n";
                cout << "  (не має абонементу)\n";
                cout << "==============================\n";
                pressEnterToContinue();
                break;
            }
            case 0:
                running = false;
                break;
            default:
                cout << "Невірний вибір!\n";
                pressEnterToContinue();
            }
        }
    }

    cout << "\nДякуємо за використання системи!\n";
    return 0;
}