#include "server.h"
#include "err_handle.h"
#include <unistd.h>  // write + sleep

// TODO: раскидать на хедеры!!

bool getMyIP (IPv4& myIP)
{
    char szBuffer[1024];

#ifdef WIN32
    WSADATA wsaData;
    WORD wVersionRequested = MAKEWORD(2, 0);
    if (::WSAStartup(wVersionRequested, &wsaData) != 0)
        return false;
#endif

    if (gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
    {
#ifdef WIN32
        WSACleanup();
#endif
        return false;
    }

    struct hostent* host = gethostbyname(szBuffer);
    if (host == NULL)
    {
#ifdef WIN32
        WSACleanup();
#endif
        return false;
    }

    // Obtain the computer's IP
    myIP.b1 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b1;
    myIP.b2 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b2;
    myIP.b3 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b3;
    myIP.b4 = ((struct in_addr*)(host->h_addr))->S_un.S_un_b.s_b4;

#ifdef WIN32
    WSACleanup();
#endif
    return true;
}

std::string get_my_ip ()
{
    IPv4 tmp{.b1 = 0, .b2 = 0, .b3 = 0, .b4 = 0};
    getMyIP(tmp);
    return std::to_string(tmp.b1) + "." + std::to_string(tmp.b2) + "." + std::to_string(tmp.b3) + "." +
           std::to_string(tmp.b4);
}

int server_test (int port)
{
    printf("server_test started...\n");

    printf("tryin to init WSAStartup on server...\n");
    WSADATA wsaData;  // содержит сведения о реализации сокетов Windows
    int iResult;      // сюда результат выполнения пишем (успех/ошибка)

    // инициализируем Winsock, проверяем на ошибки
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);  // используем winsock 2.2
    if (iResult != 0)
    {
        WSACleanup();
        printf("WSAStartup failed (server): %d\n", iResult);
        return 1;
    }
    printf("WSAStartup success on server!\n");

    struct addrinfo* result = NULL;
    struct addrinfo hints;  // удобная структура, содержит в себе информацию о сети

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    // 1-какое семейство протоколов (ipv4)
    // tcp-sock_steam, udp-dgram; 0-я хз, дефолтное значние какоето

    int iSendResult;           // сюда результат отправки пишем (успех/ошибка)
    char buf[DEFAULT_BUFLEN];  // сюда записываем инфу
    int buf_size = DEFAULT_BUFLEN;

    ZeroMemory(&hints, sizeof(hints));  // инициализируем hints, точнее - всю память нулями заполняем
    hints.ai_family = AF_INET;  // семйство адресов (проще - тип соединения типа как ipv4, bluetooth, ipv6...)
    hints.ai_socktype = SOCK_STREAM;  // тип сокета, берем дефолтный
    hints.ai_protocol = IPPROTO_TCP;  // берем протокол TCP, т.е. нам все одной пачкой придет
    hints.ai_flags = AI_PASSIVE;  // пометка для ф-ции getbyaddr, значит что мы потом будем связывать ip+port
    printf("server variables init finished!\n");

    // getaddrinfo обеспечивает независимое от протокола преобразование из имени узла ANSI в адрес
    // (крч в нормальный вид приводим адрес, и еще динамич. память выделяем на это!)
    iResult = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    printf("getaddrinfo success!\n");
    // создаем сокет ДЛЯ прослушивания подключений клиентов...
    ListenSocket = Socket(result);
    printf("creating ListenSocket success!\n");
    // связываем сокет прослушки с адресом из result
    Bind(ListenSocket, result);
    printf("Bind success!\n");

    freeaddrinfo(result);  // дальше result не нужен, т.к. все настроено

    Listen(ListenSocket, SOMAXCONN);  // ждемс

    // принимаем клиентский сокет
    printf("Waiting...\n");
    ClientSocket = Accept(ListenSocket, NULL, NULL);

    printf("Client found!\n");
    // прослушивающий закрываем, уже нашли человека
    closesocket(ListenSocket);

    // пока соединение не разорвут, получаем инфу
    // TODO: добавить Send и Read!!
    do
    {
        printf("Receiving...\n");
        iResult = recv(ClientSocket, buf, buf_size, 0);  // тут получаем
        if (iResult > 0)
        {  // если клиент не отключен
            printf("Bytes received: %d\n", iResult);

            printf("RECEIVED: %s\n", buf);  // там чета сзади прилипает в выводе, наверное служебная инфа

            // отправим на ClientSocket данные char'ы из buf, iResult штук
            printf("Sending...\n");
            iSendResult = send(ClientSocket, buf, iResult, 0);
            if (iSendResult == SOCKET_ERROR)
            {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)  // означает что клиент отключился
            printf("Closing connection ...\n");
        else
        {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    }
    while (iResult > 0);

    // вырубаем сервер полностью, мы закончили
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // чистим-чистим
    closesocket(ClientSocket);
    WSACleanup();

    return 0;
}
