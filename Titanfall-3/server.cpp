#include "settings.h"
#include "server.h"
#include <cstring>
#include <unistd.h>// write + sleep
// TODO: раскидать на хедеры!!

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
    data.reserve(NumOfPlayers);
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
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }
    // чистим-чистим
    closesocket(ClientSocket);
    WSACleanup();
}

void Server::StartListen() {
    // создаем сокет ДЛЯ прослушивания подключений клиентов...
    ListenSocket = Socket(result);
    printf("creating ListenSocket success!\n");
    // связываем сокет прослушки с адресом из result
    Bind(ListenSocket, result);
    printf("Bind success!\n");

    Listen(ListenSocket, SOMAXCONN);// ждемс

    // принимаем клиентский сокет
    printf("Waiting...\n");
    is_listening = true;
    while (curr_players_amount < NumOfPlayers) {
        ClientSocket = Accept(ListenSocket, NULL, NULL);
        printf("Client found!\n");
        Player tmp_pl;
        for (auto &pl: data) {
            if (pl.first.GetName() == "NAMEHERE") {
                closesocket(ClientSocket);
                break;
            }
        }
        //ADD
    }
    StopListen();
}

void Server::StopListen() {
    is_listening = false;
    // прослушивающий закрываем, уже нашли человека
    closesocket(ListenSocket);
}

int Server::Recv() {
    printf("SRV Receiving...\n");
    char *buf;
    iResult = recv(ClientSocket, buf, DEFAULT_BUFLEN, 0);// тут получаем
    if (iResult > 0) {                                   // если клиент не отключен
        printf("Bytes received: %d\n", iResult);
        printf("RECEIVED: %s\n", buf);// там чета сзади прилипает в выводе, наверное служебная инфа
    } else if (iResult == 0)          // означает что клиент отключился
        printf("Closing connection ...\n");
    else {
        //printf("SRV recv failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        std::string err_msg = "SRV recv failed with error: " + std::to_string(iResult);
        throw std::runtime_error(err_msg);
    }
    return 0;
}

int Server::Send(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
                 std::string &func_text_) {
    // отправим на ClientSocket данные char'ы из buf, iResult штук
    printf("SRV Sending...\n");
    const char *msg = convert(players_, obstacles_, func_text_);
    iSendResult = send(ClientSocket, msg, strlen(msg), 0);
    if (iSendResult == SOCKET_ERROR) {
        printf("SRV send failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        std::string err_msg = "SRV send failed with error: " + std::to_string(WSAGetLastError());
        throw std::runtime_error(err_msg);
    }
    printf("SRV Bytes sent: %d\n", iSendResult);
    return 0;
}

const char *Server::convert(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
                            std::string &func_text_) {
    std::string msg{};
    for (auto pl: players_) {
        std::pair<int, int> pl_cords = pl.GetCords();
        msg += pl.GetName() + delimiter + std::to_string(pl_cords.first) + delimiter +
               std::to_string(pl_cords.second) + delimiter;
    }
    msg += endchar;
    for (auto ob: obstacles_) {
        Graph_lib::Point ob_cords = ob.center;
        msg += std::to_string(ob.hole) + delimiter + std::to_string(ob_cords.x) + delimiter +
               std::to_string(ob_cords.y) + delimiter + std::to_string(ob.radius) + delimiter;
    }
    msg += endchar + func_text_ + endchar;

    char *ptr = new char[msg.size() + 1];
    strcpy(ptr, msg.c_str());
    return ptr;
}

void Server::decode(Player &pl, std::string &func,
                    bool &right, const char *buf) {
    const char *ptr = buf;
    std::string msg{};
    while (*ptr != endchar) { msg += *ptr; }
    pl.SetName(msg);
    msg.clear();
    
    while (*ptr != endchar) { msg += *ptr; }
    //func_text_ = msg;
    msg.clear();

    
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
    printf("Waiting...\n");
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
