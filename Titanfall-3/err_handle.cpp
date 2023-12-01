#include "err_handle.h"


SOCKET Socket(addrinfo *result) {
    SOCKET res = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (res == INVALID_SOCKET) {  // не получилось запустить
        perror(("socket failed #" +
                std::to_string(WSAGetLastError())).c_str());
        freeaddrinfo(result); // просто чистим память которую заняли
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return res;  // отдаем дескриптор сокета (типа ссылка/указатель или чет такое)
}

void Bind(SOCKET sock, addrinfo *result) {
    int res = bind(sock, (*result).ai_addr, int((*result).ai_addrlen));
    if (res == SOCKET_ERROR) {
        perror(("bind failed #" +
                std::to_string(WSAGetLastError())).c_str());
        freeaddrinfo(result);
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

void Listen(SOCKET sock, int backlog) {
    int res = listen(sock, backlog);
    if (res == SOCKET_ERROR) {
        perror(("listen failed #" +
                std::to_string(WSAGetLastError())).c_str());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}

SOCKET Accept(SOCKET sock, struct sockaddr *addr, socklen_t *addr_len) {
    SOCKET res = accept(sock, addr, addr_len);
    if (res == INVALID_SOCKET) {
        perror(("accept failed #" +
                std::to_string(WSAGetLastError())).c_str());
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(SOCKET sock, addrinfo *result) {
    int res = connect(sock, result->ai_addr, int(result->ai_addrlen));
    if (res == SOCKET_ERROR) { // не получилось подключиться - удаляем сокет, потому что можем
        perror("--connection failed--");
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}

void Inet_pton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);
    if (res == 0) { // недопустимую строку получили
        perror(("inet_pton failed: src does not contain a character"
               " string representing a valid net in the specified"
               " adress family\nerr #" +
                std::to_string(WSAGetLastError())).c_str());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    if (res == SOCKET_ERROR) { // все остальные ошибки
        perror(("inet_pton failed #" +
                std::to_string(WSAGetLastError())).c_str());
        WSACleanup();
        exit(EXIT_FAILURE);
    }
}
