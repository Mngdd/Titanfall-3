#ifndef TITANFALL_3_SERVER_H
#define TITANFALL_3_SERVER_H
#include <string>

struct IPv4 {
    unsigned char b1, b2, b3, b4;
};

bool getMyIP(IPv4 &myIP);

std::string get_my_ip();

int server_test(int port);

#endif//TITANFALL_3_SERVER_H
