#ifndef TITANFALL_3_SERVER_H
#define TITANFALL_3_SERVER_H
#include "environment.h"
#include "err_handle.h"
#include <string>
#include <utility>
#include <vector>
#include <map>


//todo: add send_ALL & read_ALL, prevent delete/add without permission
class Server {
public:
    Server(const int port);
    Server() = default;
    ~Server();
    void StartListen();
    void StopListen();
    int Send(const std::vector<Player> &players_, const std::vector<Obstacle> &obstacles_,
             const std::string &func_text_, bool right, const std::string &who,
             SOCKET to_who);
    void SendAll(const std::vector<Player> &players_, const std::vector<Obstacle> &obstacles_,
                const std::string &func_text_, bool right, const std::string &who);
    int Recv(Player *pl);
    void decode(Player *pl, const char *buf);
    const char *encode(const std::vector<Player> &players_, const std::vector<Obstacle> &obstacles_,
                       const std::string &func_text_, bool right, const std::string &who);
    void disconnect(Player* who);
    // можем ли сейчас менять что-то в data
    // переменная нужна для предотвращения изменений в data во время итерации по ней же
    bool can_change;

private:
    // данные игрока + его сокет
    // Player не pointer потому что мы 24/7 меняем/удаляем игроков
    // работать с памятью так быстро напряжно
    std::map<Player *, SOCKET> data;
    unsigned int curr_players_amount;
    // для предотвращения двойного открытия прослушки
    // просто проверять player_count < max не подойдет, могли отключиться после закрытия прослушки
    bool is_listening;
    // содержит сведения о реализации сокетов Windows
    WSADATA wsaData;
    // сюда результат выполнения пишем (успех/ошибка)
    int iResult;
    struct addrinfo *result;
    // удобная структура, содержит в себе информацию о сети
    struct addrinfo hints;

    SOCKET ListenSocket;
    SOCKET ClientSocket;

    int iSendResult;// сюда результат отправки пишем (успех/ошибка)
    //char buf[DEFAULT_BUFLEN];// сюда записываем инфу
    //int buf_size;
};

struct IPv4 {
    unsigned char b1, b2, b3, b4;
};

bool getMyIP(IPv4 &myIP);

std::string get_my_ip();

int server_test(int port);

#endif//TITANFALL_3_SERVER_H
