#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>

using namespace std;

// Класс для хранения информации о пользователе
class User 
{
public:
    string login;
    string password;
    string name;

    User() {}

    User(const string& login, const string& password, const string& name)
        : login(login), password(password), name(name) {}
};

// Класс для хранения информации о сообщении
class Message 
{
public:
        string sender;      // Логин отправителя
        string receiver;    // Логин получателя (если пусто, то сообщение для всех)
        string text;        // Текст сообщения

    Message(const string& sender, const string& receiver, const string& text)
        : sender(sender), receiver(receiver), text(text)
    {

    }
};

// Класс для управления чатом
class Chat 
{
private:
    map <string, User> users;           // Хранение пользователей (ключ - логин)
    vector <Message> messages;          // Хранение сообщений
    string currentUser;                // Логин текущего пользователя

public:
    Chat() : currentUser("") {}

    // Регистрация нового пользователя
    bool registerUser(const string& login, const string& password, const string& name) 
    {
        if (users.find(login) != users.end()) 
        {
            return false;  // Пользователь уже существует
        }

        users[login] = User(login, password, name);
        return true;
    }

    // Вход пользователя в систему
    bool login(const string& login, const string& password) 
    {
        auto it = users.find(login);
        if (it == users.end() || it->second.password != password) 
        {
            return false;  // Неверный логин или пароль
        }

        currentUser = login;
        return true;
    }

    // Выход пользователя из системы
    void logout() 
    {
        currentUser = "";
    }

    // Отправка сообщения
    void sendMessage(const string& receiver, const string& text) 
    {
        messages.push_back(Message(currentUser, receiver, text));
    }

    // Вывод всех сообщений для текущего пользователя
    void showMessages() 
    {
        cout << "\n=== Сообщения ===" << endl;
        bool hasMessages = false;

        for (const auto& message : messages) 
        {
            // Показываем общие сообщения и личные сообщения для текущего пользователя
            if (message.receiver.empty() || message.receiver == currentUser || message.sender == currentUser) 
            {
                hasMessages = true;

                // Формируем строку типа сообщения
                string messageType;
                if (message.receiver.empty()) 
                {
                    messageType = "(всем)";
                }
                else if (message.receiver == currentUser) 
                {
                    messageType = "(лично вам)";
                }
                else 
                {
                    messageType = "(лично для " + message.receiver + ")";
                }

                cout 
                    << message.sender << " " << messageType << ": "
                    << message.text << endl;
            }
        }

        if (!hasMessages) 
        {
            cout << "Нет сообщений." << endl;
        }
    }

    // Вывод списка пользователей
    void showUsers() 
    {
        cout << "\n=== Пользователи ===" << endl;
        for (const auto& pair : users) 
        {
            cout << pair.first << " (" << pair.second.name << ")" << endl;
        }
    }

    // Проверка, вошел ли пользователь в систему
    bool isLoggedIn() const 
    {
        return !currentUser.empty();
    }

    // Получение имени текущего пользователя
    string getCurrentUserName() const 
    {
        if (!isLoggedIn()) return "";
        return users.at(currentUser).name;
    }
};

// Обработка исключений
class ChatException : public exception 
{
private:
    string message;

public:
    ChatException(const string& msg) : message(msg) {}

    const char* what() const noexcept override 
    {
        return message.c_str();
    }
};

int main() 
{
    system("chcp 1251");
    Chat chat;
    int choice;

    while (true) 
    {
        if (!chat.isLoggedIn()) 
        {
            // Меню для неавторизованных пользователей
            cout << "\n=== Чат-программа ===" << endl;
            cout << "1. Регистрация" << endl;
            cout << "2. Вход" << endl;
            cout << "0. Выход" << endl;
            cout << "Выберите действие: ";

            try 
            {
                cin >> choice;
                cin.ignore(); // Очистка буфера ввода
            }

            catch (...) 
            {
                cout << "Ошибка ввода. Пожалуйста, введите число." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 1) 
            {
                // Регистрация
                string login, password, name;

                cout << "Введите логин: ";
                getline(cin, login);

                cout << "Введите пароль: ";
                getline(cin, password);

                cout << "Введите имя: ";
                getline(cin, name);

                if (chat.registerUser(login, password, name)) 
                {
                    cout << "Регистрация успешна!" << endl;
                }
                else 
                {
                    cout << "Пользователь с таким логином уже существует." << endl;
                }

            }

            else if (choice == 2) 
            {
                // Вход
                string login, password;

                cout << "Введите логин: ";
                getline(cin, login);

                cout << "Введите пароль: ";
                getline(cin, password);

                if (chat.login(login, password)) 
                {
                    cout << "Вход выполнен успешно!" << endl;
                }
                else 
                {
                    cout << "Неверный логин или пароль." << endl;
                }

            }

            else if (choice == 0) 
            {
                // Выход из программы
                cout << "До свидания!" << endl;
                break;

            }

            else 
            {
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }

        }

        else 
        {
            // Меню для авторизованных пользователей
            cout << "\n=== Чат (" << chat.getCurrentUserName() << ") ===" << endl;
            cout << "1. Отправить сообщение всем" << endl;
            cout << "2. Отправить личное сообщение" << endl;
            cout << "3. Просмотреть сообщения" << endl;
            cout << "4. Список пользователей" << endl;
            cout << "5. Выйти из аккаунта" << endl;
            cout << "0. Выход из программы" << endl;
            cout << "Выберите действие: ";

            try 
            {
                cin >> choice;
                cin.ignore(); // Очистка буфера ввода
            }

            catch (...) 
            {
                cout << "Ошибка ввода. Пожалуйста, введите число." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            if (choice == 1) 
            {
                // Отправка сообщения всем
                string text;
                cout << "Введите сообщение: ";
                getline(cin, text);
                chat.sendMessage("", text); // Пустой получатель - сообщение для всех
                cout << "Сообщение отправлено всем." << endl;

            }

            else if (choice == 2) 
            {
                // Отправка личного сообщения
                string receiver, text;

                cout << "Введите логин получателя: ";
                getline(cin, receiver);

                cout << "Введите сообщение: ";
                getline(cin, text);

                chat.sendMessage(receiver, text);
                cout << "Сообщение отправлено." << endl;

            }

            else if (choice == 3) 
            {
                // Просмотр сообщений
                chat.showMessages();

            }

            else if (choice == 4) 
            {
                // Список пользователей
                chat.showUsers();

            }

            else if (choice == 5) 
            {
                // Выход из аккаунта
                chat.logout();
                cout << "Вы вышли из аккаунта." << endl;

            }

            else if (choice == 0) 
            {
                // Выход из программы
                cout << "До свидания!" << endl;
                break;

            }

            else 
            {
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        }
    }

    return 0;
}