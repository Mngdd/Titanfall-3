#ifndef SETTINGS_H
#define SETTINGS_H

#include <Graph_lib/Graph.h>
#include <string>

// выход из игры
inline bool wanna_exit = false;

// ОКНА WINDOWS

// Высота поля
const int FieldWidth = 1200;
// Ширина поля
const int FieldHeight = 720;
// Высота меню во время игры
const int InputHeight = 145;
// Ширина меню во время игры
const int InputWidth = 350;
// размеры окна создания сервера
const int HostWin_x = 325;
const int HostWin_y = 350;

// размеры окна подключения к серверу
const int ConnWin_x = 300;
const int ConnWin_y = 90;

// переменные для сдвига кнопок в меню
const unsigned int bg_offset = 35;
const unsigned int btn_begin = bg_offset + 40;
const unsigned int btn_betw = 50;

// РАСКРАСКА

inline Graph_lib::Color HugeObsColor = Graph_lib::Color::black;
inline Graph_lib::Color MediumObsColor = Graph_lib::Color::black;
inline Graph_lib::Color SmallObsColor = Graph_lib::Color::black;

// Случайный цвет игроков
inline bool RainbowPlayers = true;  // FIXME: не работает
inline Graph_lib::Color PlayerColor = Graph_lib::Color::green;

// Color Team1PlayerColor = Color::blue;
// Color Team2PlayerColor = Color::red;

// состояние игры
enum game_state
{
    FIRE = 0,
    RESPAWN = 1,
    LEAVE = 2
};

// СЕРВЕР
const std::string GameName = "Graph War v0.01 pre-pre-alpha indev";
// наш айпи, не const, вдруг он поменяется (впн мб, я не уверен)
inline std::string MY_IP = "NONE";
// айпи сервера, на который подключаться будем
inline std::string CONN_IP = "NONE";
// порт сервера, на который подключаться будем
inline int PORT = -1;
// мне создавать сервер?
inline bool IM_A_HOST = false;

// кол-во игроков
inline int NumOfPlayers = 2;
// Радиус тела игрока
const int PlayerRad = 10;
// Размер ника (font_size)
const int NickSize = 15;
// Имя нашего игрока
inline std::string UserNick = "Player";

// радиус бабаха
const int WhiteObsRad = 10;

#endif
