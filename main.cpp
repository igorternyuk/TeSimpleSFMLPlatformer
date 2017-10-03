/* 
 * File:   main.cpp
 * Author: igor
 *
 * Created on 29-09-2017, 8:38
 */

#include "Game.hpp"
#include <stdexcept>
#include <iostream>

int main()
{
    try
    {
        Game game;
        game.gameLoop();
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }
 
    return 0;
}

