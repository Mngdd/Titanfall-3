#ifndef TITANFALL_3_CLIENT_H
#define TITANFALL_3_CLIENT_H
#include "err_handle.h"
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


public:
    Client(const std::string& ip_, const int port);
    ~Client();
    //void Connect(std::string &ip, int port);
    // возвращает кол-во 
    int Send(const std::string& name, const std::string& func, bool right);
    int Recv(); // FIXME: RETURN NEEDED OBJECTS, NOT VOID!!!
};

int client_test(const char ip[], int port);

#endif//TITANFALL_3_CLIENT_H
