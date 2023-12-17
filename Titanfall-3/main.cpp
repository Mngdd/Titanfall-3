#include "game_logic.h"
#include <exception>
#include <iostream>

int main ()
{
    try
    {
        game_loop();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const char* msg)
    {
        std::cerr << "input error " << msg << std::endl;
    }
    catch (...)
    {
        std::cerr << "UNKNOWN ERR" << std::endl;
    }
}