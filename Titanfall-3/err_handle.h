#ifndef TITANFALL_3_ERR_HANDLE_H
#define TITANFALL_3_ERR_HANDLE_H

#include <WinSock2.h> // замена linux <sys/socket.h>, тут функция сокет определена
#include <ws2tcpip.h> // socketlen_t
#include <cstdlib> // exit + perror

// Создаем сокет и обрабатываем ошибки, если будут
int Socket(int domain, int type, int protocol);

// Привязываем сокет и обрабатываем ошибки, если будут
void Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// Слушаем... (+ ошибки)
void Listen(int sockfd, int backlog);

// Вернет файловый дескриптор, через который можно общаться с клиентом (+ ошибки)
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

// обертка для connect
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

// обертка для inet_pton, преобразует сетевой адрес IPv4 или IPv6
// в стандартной форме представления текста в числовую двоичную форму
void Inet_pton(int af, const char *src, void *dst);
#endif //TITANFALL_3_ERR_HANDLE_H
