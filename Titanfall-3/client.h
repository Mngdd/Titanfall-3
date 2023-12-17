#ifndef TITANFALL_3_CLIENT_H
#define TITANFALL_3_CLIENT_H
#include "err_handle.h"
#include <cstdio>
#include <map>
#include <string>
#include <unistd.h>  // write
#include <utility>
#include <vector>

using container = std::map<std::string, std::vector<std::pair<std::string, std::pair<int, int>>>>;

// class Client {
// private:
//     WSADATA wsaData;
//     SOCKET ConnectSocket;
//     struct addrinfo *result;
//     const char *sendbuf;
//     // буффер размером DEFAULT_BUFLEN, мб поменяем
//     char buf[DEFAULT_BUFLEN];
//     // содержит результат (int) исполнения
//     int iResult;
//     // размер буфера приема

// public:
//     Client();
//     ~Client();
//     void Connect(std::string &ip, int port);
//     void Send(std::vector < Player & pl,
//               std::vector<Obstacle> &Obst);
//     container Recv();
// };

int client_test (const char ip[], int port);

#endif  // TITANFALL_3_CLIENT_H
