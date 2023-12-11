#ifndef TITANFALL_3_CLIENT_H
#define TITANFALL_3_CLIENT_H
#include "err_handle.h"
#include <cstdio>
#include <map>
#include <utility>
#include <string>
#include <unistd.h>// write
#include <vector>

using container = std::map<
        std::string,
        std::vector<
                std::pair<
                        std::string,
                        std::pair<int, int>>>>;

// class Client {
// private:
//     WSADATA wsaData;
//     SOCKET ConnectSocket;
//     struct addrinfo *result;
//     const char *sendbuf;
//     char buf[DEFAULT_BUFLEN];//
//     int iResult;
//     int buf_size = DEFAULT_BUFLEN;

// public:
// Client() = default;
// //Client();
// ~Client();
//     void Connect(std::string& ip, int port);
//     void Send(std::vector<Player& pl,
//     std::vector<Obstacle>& Obst);
// container Recv();
// };

int client_test(const char ip[], int port);

#endif//TITANFALL_3_CLIENT_H
