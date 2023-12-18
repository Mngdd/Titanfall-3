#include "client.h"
#include "settings.h"
#include <Graph_lib/Point.h>
#include <iostream>
#include <stdexcept>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
// #pragma comment(lib, "Ws2_32.lib")
// #pragma comment(lib, "Mswsock.lib")
// #pragma comment(lib, "AdvApi32.lib")// TODO: помогите, откуда их в симейке привязывать...


Client::Client() {
    ConnectSocket = INVALID_SOCKET;
    result = NULL;
    ptr = NULL;

    //const char *sendbuf; //todo: не забыть отправлять через sendbuf

    // init Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        //printf("WSAStartup failed with error: %d\n", iResult);
        std::string err_msg = "WSAStartup failed with error: " + std::to_string(iResult);
        throw std::runtime_error(err_msg);
    }

    ZeroMemory(&hints, sizeof(hints));// чистим кусок памяти от мусора, все в нулях
    hints.ai_family = AF_UNSPEC;      // без семейства
    hints.ai_socktype = SOCK_STREAM;  // тип сокета, берем дефолтный
    hints.ai_protocol = IPPROTO_TCP;  // берем протокол TCP, т.е. нам все одной пачкой придет
}

Client::~Client() {
    std::cout << "CLIENT IS SHUTTIN DOWN!\n";
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        // change da wolrd, my final message, goodbye
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }
    closesocket(ConnectSocket);
    WSACleanup();
}

void Client::Connect_to(const std::string &ip_, const int port) {
    // getaddrinfo обеспечивает независимое от протокола преобразование из имени узла ANSI в адрес
    // (крч в нормальный вид приводим адрес, и еще динамич. память выделяем на это!)
    iResult = getaddrinfo(ip_.c_str(), std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0) {
        //printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        std::string err_msg = "getaddrinfo failed with error: " + std::to_string(iResult);
        throw std::runtime_error(err_msg);
    }
    // подключаемся до тех пор, пока нас не подключит
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // создаем сокет чтоб к серверу подключиться
        ConnectSocket = Socket(ptr);

        // пытаемся подключиться
        Connect(ConnectSocket, ptr);// если не удалось - отключаем сокет
        if (ConnectSocket == INVALID_SOCKET) { continue; }
        break;
    }

    freeaddrinfo(result);// мы настроились уже, не нужен он больше

    if (ConnectSocket == INVALID_SOCKET) {
        //printf("Unable to connect to server!\n");
        WSACleanup();
        throw std::runtime_error("Unable to connect to server!");
    }
}

int Client::Send(const std::string &name, bool respawn, bool alive,
                 const std::string &func, bool right) {
    std::string msg = name + delimiter + std::to_string(respawn) + delimiter +
                      std::to_string(alive) + delimiter + func + delimiter +
                      std::to_string(right) + endchar;
    const char *data = msg.c_str();
    iResult = send(ConnectSocket, data, (int) strlen(data), 0);
    if (iResult == SOCKET_ERROR) {
        //printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        std::string err_msg = "send failed with error: " + std::to_string(WSAGetLastError());
        throw std::runtime_error(err_msg);
    }
    return iResult;
}

int Client::Recv(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
                 std::string &func_text_, bool& right, std::string &who) {
    char buf[DEFAULT_BUFLEN];
    iResult = recv(ConnectSocket, buf, DEFAULT_BUFLEN, 0);
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);
    } else if (iResult == 0) {
        //throw std::runtime_error("Connection closed\n");
        printf("Connection closed\n");
        return 0;
    } else {
        std::string err_msg = "recv failed with error: " + std::to_string(WSAGetLastError());
        throw std::runtime_error(err_msg);
    }

    decode(players_, obstacles_, func_text_, right, who, buf);
    return iResult;
}

// молимся что оно работает и я не потерял нигде ++ptr ;(
void Client::decode(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
                    std::string &func_text_, bool& right, std::string &who, const char *buf) {
    const char *ptr = buf;
    std::string msg{};
    players_.clear();
    obstacles_.clear();

    while (*ptr != endchar) {// players_
        std::string name_;
        int x_, y_;// с '_' чтоб просто показать что это копии, хз, вроде так лучше выглядит
        bool respawn_, alive_;
        // get name
        for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
        name_ = msg;
        msg.clear();

        // x
        ++ptr;
        for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
        x_ = std::stoi(msg);
        msg.clear();
        // y
        ++ptr;
        for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
        y_ = std::stoi(msg);
        msg.clear();

        // resp+alive
        ++ptr;
        respawn_ = *ptr == TRUE_BOOL;
        ptr += 2;
        alive_ = *ptr == TRUE_BOOL;
        ++ptr;

        players_.emplace_back(name_, x_, y_);
        if (alive_ || respawn_) {
            players_[-1].Revive();
        } else {
            players_[-1].Kill();
        }
    }

    ++ptr;
    while (*ptr != endchar) {// players_
        int x_, y_, r_;
        bool hole_;
        // get hole
        hole_ = *ptr == TRUE_BOOL;
        ptr += 2;

        // x
        for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
        x_ = std::stoi(msg);
        msg.clear();
        // y
        ++ptr;
        for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
        y_ = std::stoi(msg);
        msg.clear();
        // r
        ++ptr;
        for (; (*ptr != delimiter) && (*ptr != endchar); ++ptr) { msg += *ptr; }
        r_ = std::stoi(msg);
        msg.clear();

        obstacles_.emplace_back(Graph_lib::Point{x_, y_}, r_, hole_);
    }

    ++ptr;
    for (; *ptr != delimiter; ++ptr) { msg += *ptr; }
    func_text_ = msg;
    msg.clear();

    ++ptr;
    right = *ptr == TRUE_BOOL;
    ptr += 2;
    for (; *ptr != endchar; ++ptr) { msg += *ptr; }
    who = msg;
    msg.clear();
}

// Тут все почти аналогично server.cpp, комментирую только новое
int client_test(const char ip[], int port) {
    WSADATA wsaData;// содержит сведения о реализации сокетов Windows
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;

    const char *sendbuf = "HOLA AMIGOS!";// отправляемая инфа

    char buf[DEFAULT_BUFLEN];//
    int iResult;
    // int buf_size = DEFAULT_BUFLEN; // useless

    // init Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));// чистим кусок памяти от мусора, все в нулях
    hints.ai_family = AF_UNSPEC;      // без семейства
    hints.ai_socktype = SOCK_STREAM;  // тип сокета, берем дефолтный
    hints.ai_protocol = IPPROTO_TCP;  // берем протокол TCP, т.е. нам все одной пачкой придет

    // getaddrinfo обеспечивает независимое от протокола преобразование из имени узла ANSI в адрес
    // (крч в нормальный вид приводим адрес, и еще динамич. память выделяем на это!)
    iResult = getaddrinfo(ip, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // подключаемся до тех пор, пока нас не подключит
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // создаем сокет чтоб к серверу подключиться
        ConnectSocket = Socket(ptr);

        // пытаемся подключиться
        Connect(ConnectSocket, ptr);// если не удалось - отключаем сокет
        if (ConnectSocket == INVALID_SOCKET) { continue; }
        break;
    }

    freeaddrinfo(result);// мы настроились уже, не нужен он больше

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // отправляем...
    //TODO: добавить Send и Read!!
    iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %d\n", iResult);

    // мы отправили все что хотели, отключаемся от отправки
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        // change da wolrd, my final message, goodbye
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // получаем до тех пор, пока соединение не разорвется
    do {
        iResult = recv(ConnectSocket, buf, DEFAULT_BUFLEN, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // чистим, иначе сокеты будут висеть занятыми
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}