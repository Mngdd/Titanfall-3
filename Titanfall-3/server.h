#ifndef TITANFALL_3_SERVER_H
#define TITANFALL_3_SERVER_H
#include "environment.h"
#include "err_handle.h"
#include <string>
#include <utility>
#include <vector>


//todo: add reader!
//FIXME: support more than 2 players!
class Server {
public:
    Server(const int port);
    ~Server();
    void StartListen();
    void StopListen();
    int Send(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
             std::string &func_text_);
    int Recv();
    void decode(Player &pl, std::string &func,
                bool &right, const char *buf);

private:
    std::vector<std::pair<Player, SOCKET>> data;// данные игрока + его сокет
    unsigned int curr_players_amount;
    bool is_listening;
    WSADATA wsaData;// содержит сведения о реализации сокетов Windows
    int iResult;    // сюда результат выполнения пишем (успех/ошибка)
    struct addrinfo *result;
    struct addrinfo hints;// удобная структура, содержит в себе информацию о сети

    SOCKET ListenSocket;
    SOCKET ClientSocket;

    int iSendResult;// сюда результат отправки пишем (успех/ошибка)
    //char buf[DEFAULT_BUFLEN];// сюда записываем инфу
    //int buf_size;

    const char *convert(std::vector<Player> &players_, std::vector<Obstacle> &obstacles_,
                        std::string &func_text_);
    void decode(std::string &name, std::string &func,
                bool &right, const char *buf);
};

struct IPv4 {
    unsigned char b1, b2, b3, b4;
};

bool getMyIP(IPv4 &myIP);

std::string get_my_ip();

int server_test(int port);

#endif//TITANFALL_3_SERVER_H
