// у меня не робят закоменченные инклюды лол
#include "level_gen.h"
//#include "main.h"
//
//#include <Graph_lib/Graph.h>
//#include <Graph_lib/Simple_window.h>
#include "server.h"
#include "client.h"
#include <string>
#include <iostream>
#include <thread>
//using namespace Graph_lib;

int main() 
try
{
    std::string ip = "127.0.0.1";
    int port = 55355;

    std::thread srv(server_test, port); // starts rn
    std::thread cli(client_test, ip.c_str(), port);

    // waits for finish its exec
    srv.join();
    cli.join();
}
catch (exception& e)
{
    cerr << "Error: " << e.what() << endl;
    return 1;
}
catch (...)
{
    cerr << "Unknown error " << endl;
    return 2;
}