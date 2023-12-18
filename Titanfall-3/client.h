#ifndef TITANFALL_3_CLIENT_H
#define TITANFALL_3_CLIENT_H
#include "err_handle.h"
#include "environment.h"
#include <cstdio>
#include <map>
#include <string>
#include <unistd.h>// write
#include <utility>
#include <vector>

using container = std::map<
        std::string,
        std::vector<
                std::pair<
                        std::string,
                        std::pair<int, int>>>>;

class Client {
private:
    WSADATA wsaData;
    SOCKET ConnectSocket;
    struct addrinfo *result, *ptr, hints;
    const char *sendbuf;
    // буффер размером DEFAULT_BUFLEN, мб поменяем
    //char buf[DEFAULT_BUFLEN]; //todo: не забыть юзать
    // содержит результат (int) исполнения
    int iResult;
    // размер буфера приема
    void decode(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
                std::string &func_text_, bool& right, std::string &who, const char *buf);

public:
    Client();
    ~Client();
    //void Connect(std::string &ip, int port);
    // возвращает кол-во
    void Connect_to(const std::string &ip_, const int port);
    int Send(const std::string &name, bool respawn, bool alive,
             const std::string &func, bool right);
    int Recv(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
             std::string &func_text_, bool& right, std::string &who);
};

int client_test(const char ip[], int port);

#endif//TITANFALL_3_CLIENT_H
