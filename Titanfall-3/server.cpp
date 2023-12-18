#include "server.h"
#include "settings.h"
#include <cstring>
#include <unistd.h>// write + sleep
#include <thread>

bool getMyIP(IPv4 &myIP) {
    char szBuffer[1024];

    #ifdef WIN32
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 0);
    if (::WSAStartup(wVersionRequested, &wsaData) != 0)
        return false;
    #endif
    if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR) {
    #ifdef WIN32
        WSACleanup();
    #endif
        return false;
    }

    struct hostent *host = gethostbyname(szBuffer);
    if (host == NULL) {
    #ifdef WIN32
        WSACleanup();
    #endif
        return false;
    }

    //Obtain the computer's IP
    myIP.b1 = ((struct in_addr *) (host->h_addr))->S_un.S_un_b.s_b1;
    myIP.b2 = ((struct in_addr *) (host->h_addr))->S_un.S_un_b.s_b2;
    myIP.b3 = ((struct in_addr *) (host->h_addr))->S_un.S_un_b.s_b3;
    myIP.b4 = ((struct in_addr *) (host->h_addr))->S_un.S_un_b.s_b4;

    #ifdef WIN32
    WSACleanup();
    #endif
    return true;
}

std::string get_my_ip() {
    IPv4 tmp{.b1 = 0, .b2 = 0, .b3 = 0, .b4 = 0};
    getMyIP(tmp);
    return std::to_string(tmp.b1) + "." + std::to_string(tmp.b2) +
           "." + std::to_string(tmp.b3) +
           "." + std::to_string(tmp.b4);
}

Server::Server(const int port) {
    curr_players_amount = 0;
    is_listening = false;
    can_change = true;

    //data.reserve(NumOfPlayers);
    printf("server_test started...\n");
    printf("tryin to init WSAStartup on server...\n");

    // инициализируем Winsock, проверяем на ошибки
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);// используем winsock 2.2
    if (iResult != 0) {
        WSACleanup();
        //printf("WSAStartup failed (server): %d\n", iResult);
        std::string err_msg = "WSAStartup failed (server): " + std::to_string(iResult);
        throw std::runtime_error(err_msg);
    }
    printf("WSAStartup success on server!\n");

    result = NULL;

    ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;
    // 1-какое семейство протоколов (ipv4)
    // tcp-sock_steam, udp-dgram; 0-я хз, дефолтное значние какоето

    ZeroMemory(&hints, sizeof(hints));// инициализируем hints, точнее - всю память нулями заполняем
    hints.ai_family = AF_INET;        // семйство адресов (проще - тип соединения типа как ipv4, bluetooth, ipv6...)
    hints.ai_socktype = SOCK_STREAM;  // тип сокета, берем дефолтный
    hints.ai_protocol = IPPROTO_TCP;  // берем протокол TCP, т.е. нам все одной пачкой придет
    hints.ai_flags = AI_PASSIVE;      // пометка для ф-ции getbyaddr, значит что мы потом будем связывать ip+port
    printf("server variables init finished!\n");

    // getaddrinfo обеспечивает независимое от протокола преобразование из имени узла ANSI в адрес
    // (крч в нормальный вид приводим адрес, и еще динамич. память выделяем на это!)
    iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        std::string err_msg = "getaddrinfo failed with error: " + std::to_string(iResult);
        throw std::runtime_error(err_msg);
    }
    printf("getaddrinfo success!\n");
}

Server::~Server() {
    std::cout << "SERVER IS SHUTTIN DOWN!\n";
    // вырубаем сервер полностью, мы закончили
    freeaddrinfo(result);// Освобождаем памятьб

    // чистим-чистим
    for (auto& [key, val] : data) {
        disconnect(key);
    }
    WSACleanup();
}

void Server::StartListen() {
    // создаем сокет ДЛЯ прослушивания подключений клиентов...
    printf("creating ListenSocket...\n");
    ListenSocket = Socket(result);
    printf("creating ListenSocket success!\n");
    // связываем сокет прослушки с адресом из result
    Bind(ListenSocket, result);
    printf("Bind success!\n");

    Listen(ListenSocket, SOMAXCONN);// ждемс

    // принимаем клиентский сокет
    is_listening = true;
    while ((curr_players_amount < NumOfPlayers) && is_listening) {
        printf("SRV Waiting...\n");
        ClientSocket = Accept(ListenSocket, NULL, NULL);
        printf("Client found!\n");

        Player *ptr = new Player(UserNick);
        Recv(ptr);

        bool accepted = true;
        for (auto& [key, val] : data) {                           // проверяем, есть ли такой ник на серве
            if (key->GetName() == ptr->GetName()) {// ник все-таки занят
                closesocket(ClientSocket);
                accepted = false;
                delete ptr;
                break;
            }
        }
        if (accepted) {// ADD
            // ждем пока разрешат, точно уйдет в бесконечность (>1 сек уж точно)
            while (!can_change);
            data[ptr] = ClientSocket;
        }
    }
    StopListen();
}

void Server::StopListen() {
    is_listening = false;
    // прослушивающий закрываем, уже нашли человека
    closesocket(ListenSocket);
}

int Server::Recv(Player *pl) {
    char buf[DEFAULT_BUFLEN];
    printf("SRV Receiving...\n");
    
    SOCKET from_who = data[pl];
    DWORD max_timeout = SOCKET_TIMEOUT_SEC * 1000;
    setsockopt(from_who, SOL_SOCKET, SO_RCVTIMEO, (char*)&max_timeout, sizeof(max_timeout));
    
    iResult = recv(from_who, buf, DEFAULT_BUFLEN, 0);// тут получаем
    if (iResult > 0) {                               // если клиент не отключен
        printf("Bytes received: %d\n", iResult);
        printf("RECEIVED: %s\n", buf);// там чета сзади прилипает в выводе, наверное служебная инфа
    } else if (iResult == 0) {        // означает что клиент отключился
        printf("SRV Client %s disconnected...\n", pl->GetName().c_str());
        data.erase(pl);
        return 0;
    } else if (WSAGetLastError() != WSAETIMEDOUT) {
        printf("SRV Client %s timeout reached...\n", pl->GetName().c_str());
        disconnect(pl);
        return 0;
    } else {
        
        // printf("SRV recv failed with error: %d\n", WSAGetLastError());
        closesocket(from_who);
        WSACleanup();
        std::string err_msg = "SRV recv failed with error: " + std::to_string(iResult);
        throw std::runtime_error(err_msg);
    }

    decode(pl, buf);
    return iResult;
}

int Server::Send(const std::vector<Player> &players_, const std::vector<Obstacle> &obstacles_,
                 const std::string &func_text_, bool right, const std::string &who,
                 SOCKET to_who) {// все по ссылке - вдруг мы успеем чет нажать
    // отправим на ClientSocket данные char'ы из buf, iResult штук
    printf("SRV Sending...\n");
    const char *msg = encode(players_, obstacles_, func_text_, right, who);
    iSendResult = send(to_who, msg, strlen(msg), 0);
    if (iSendResult == SOCKET_ERROR) {
        printf("SRV send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        std::string err_msg = "SRV send failed with error: " + std::to_string(WSAGetLastError());
        throw std::runtime_error(err_msg);
    }
    printf("SRV Bytes sent: %d\n", iSendResult);
    return iSendResult;
}

void Server::SendAll(const std::vector<Player> &players_, const std::vector<Obstacle> &obstacles_,
                    const std::string &func_text_, bool right, const std::string &who) {
    for (auto& [key, val] : data) {
        Send(players_, obstacles_, func_text_, right, who, val);
    }
}

const char *Server::encode(const std::vector<Player> &players_, const std::vector<Obstacle> &obstacles_,
                           const std::string &func_text_, bool right, const std::string &who) {
    std::string msg{};
    // single client: name$x$y$respawn$alive... ($ if next player exists else ;)
    for (size_t i = 0; i < players_.size(); ++i) {
        std::pair<int, int> pl_cords = players_[i].GetCords();
        msg += players_[i].GetName() + delimiter + std::to_string(pl_cords.first) +
               delimiter + std::to_string(pl_cords.second) +
               delimiter + std::to_string(players_[i].awaits_respawn) +
               delimiter + std::to_string(players_[i].IsAlive());
        if ((i + 1) != players_.size()) {
            msg += delimiter;
        } else {
            msg += endchar;
        }
    }

    // single obstacle: hole(bool)$x$y$radius... ($ if next obstacle exists else ;)
    for (size_t i = 0; i < obstacles_.size(); ++i) {
        Graph_lib::Point ob_cords = obstacles_[i].center;
        msg += std::to_string(obstacles_[i].hole) + delimiter + std::to_string(ob_cords.x) + delimiter +
               std::to_string(ob_cords.y) + delimiter + std::to_string(obstacles_[i].radius);
        if ((i + 1) != obstacles_.size()) {
            msg += delimiter;
        } else {
            msg += endchar;
        }
    }

    // important data about current turn: func$right$who;
    msg += func_text_ + delimiter + std::to_string(right) + delimiter + who + endchar;

    // put into c-string...
    char *ptr = new char[msg.size() + 1];
    strcpy(ptr, msg.c_str());
    return ptr;
}

void Server::decode(Player *pl, const char *buf) {
    const char *ptr = buf;
    std::string msg{};

    // get name
    for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
    pl->SetName(msg);
    msg.clear();
    ++ptr;

    // get respawn+alive and respawn if needed else kill
    pl->awaits_respawn = *ptr == TRUE_BOOL;
    ptr += 2;
    bool alive_ = *ptr == TRUE_BOOL;
    ptr += 2;
    if (alive_ || pl->awaits_respawn) {
        pl->Revive();
    } else {
        pl->Kill();
    }

    // get func
    for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
    pl->func = msg;
    msg.clear();
    ++ptr;

    // get right(bool)
    pl->right = *ptr == TRUE_BOOL;
}

void Server::disconnect(Player* who) {
    iResult = shutdown(data[who], SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }
    closesocket(data[who]);// free mem
    data.erase(who);
}

int server_test(int port) {
    printf("server_test started...\n");

    printf("tryin to init WSAStartup on server...\n");
    WSADATA wsaData;// содержит сведения о реализации сокетов Windows
    int iResult;    // сюда результат выполнения пишем (успех/ошибка)

    // инициализируем Winsock, проверяем на ошибки
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);// используем winsock 2.2
    if (iResult != 0) {
        WSACleanup();
        printf("WSAStartup failed (server): %d\n", iResult);
        return 1;
    }
    printf("WSAStartup success on server!\n");

    struct addrinfo *result = NULL;
    struct addrinfo hints;// удобная структура, содержит в себе информацию о сети

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    // 1-какое семейство протоколов (ipv4)
    // tcp-sock_steam, udp-dgram; 0-я хз, дефолтное значние какоето

    int iSendResult;         // сюда результат отправки пишем (успех/ошибка)
    char buf[DEFAULT_BUFLEN];// сюда записываем инфу
    int buf_size = DEFAULT_BUFLEN;

    ZeroMemory(&hints, sizeof(hints));// инициализируем hints, точнее - всю память нулями заполняем
    hints.ai_family = AF_INET;        // семйство адресов (проще - тип соединения типа как ipv4, bluetooth, ipv6...)
    hints.ai_socktype = SOCK_STREAM;  // тип сокета, берем дефолтный
    hints.ai_protocol = IPPROTO_TCP;  // берем протокол TCP, т.е. нам все одной пачкой придет
    hints.ai_flags = AI_PASSIVE;      // пометка для ф-ции getbyaddr, значит что мы потом будем связывать ip+port
    printf("server variables init finished!\n");

    // getaddrinfo обеспечивает независимое от протокола преобразование из имени узла ANSI в адрес
    // (крч в нормальный вид приводим адрес, и еще динамич. память выделяем на это!)
    iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    printf("getaddrinfo success!\n");
    // создаем сокет ДЛЯ прослушивания подключений клиентов...
    ListenSocket = Socket(result);
    printf("creating ListenSocket success!\n");
    // связываем сокет прослушки с адресом из result
    Bind(ListenSocket, result);
    printf("Bind success!\n");

    freeaddrinfo(result);// дальше result не нужен, т.к. все настроено

    Listen(ListenSocket, SOMAXCONN);// ждемс

    // принимаем клиентский сокет
    printf("SRV Waiting...\n");
    ClientSocket = Accept(ListenSocket, NULL, NULL);

    printf("Client found!\n");
    // прослушивающий закрываем, уже нашли человека
    closesocket(ListenSocket);

    // пока соединение не разорвут, получаем инфу
    //TODO: добавить Send и Read!!
    do {
        printf("SRV Receiving...\n");
        iResult = recv(ClientSocket, buf, buf_size, 0);// тут получаем
        if (iResult > 0) {                             // если клиент не отключен
            printf("Bytes received: %d\n", iResult);

            printf("RECEIVED: %s\n", buf);// там чета сзади прилипает в выводе, наверное служебная инфа

            // отправим на ClientSocket данные char'ы из buf, iResult штук
            printf("Sending...\n");
            iSendResult = send(ClientSocket, buf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("SRV send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("SRV Bytes sent: %d\n", iSendResult);
        } else if (iResult == 0)// означает что клиент отключился
            printf("Closing connection ...\n");
        else {
            printf("SRV recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }

    } while (iResult > 0);

    // вырубаем сервер полностью, мы закончили
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // чистим-чистим
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
