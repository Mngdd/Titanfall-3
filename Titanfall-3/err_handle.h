#ifndef TITANFALL_3_ERR_HANDLE_H
#define TITANFALL_3_ERR_HANDLE_H
#define DEFAULT_BUFLEN 512

#include <WinSock2.h> // замена linux <sys/socket.h>, тут функция сокет определена
#include <windows.h>
#include <ws2tcpip.h> // socketlen_t
#include <cstdlib> // exit + perror
#include <string>

// Создаем сокет и обрабатываем ошибки, если будут
SOCKET Socket(addrinfo *result);

// обертка для bind (для обработки ошибок), привязываем к сокету локальный адрес
void Bind(SOCKET sock, addrinfo *result);

// обертка для listen (для обработки ошибок), переводит сокет в режим прослушивания, НО НЕ ЖДЕТ!
void Listen(SOCKET sock, int backlog);

// обертка для accept
// ждет первого подключения и возвращает ссылку на сокет первого в очереди клиента
SOCKET Accept(SOCKET sock, struct sockaddr *addr, socklen_t *addr_len);

// обертка для connect (для обработки ошибок), соединяемся с сокетом sock
void Connect(SOCKET sock, addrinfo *result);

// обертка для inet_pton (для обработки ошибок), преобразует сетевой адрес IPv4 или IPv6
// в стандартной форме представления текста в числовую двоичную форму
void Inet_pton(int af, const char *src, void *dst); //TODO: скорее всего не нужен больше, удалить мб
//TODO: добавить Send и Read!!
#endif //TITANFALL_3_ERR_HANDLE_H
