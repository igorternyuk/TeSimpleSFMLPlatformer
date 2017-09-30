/* 
 * File:   Game.hpp
 * Author: igor
 *
 * Created on 30-09-2017 г., 12:11
 */

#ifndef GAME_HPP
#define GAME_HPP

#include "Player.hpp"
#include <vector>
#include <string>
#include <memory>

class Game {
public:
    explicit Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
    void run();
    virtual ~Game();
private:
    const std::string TITLE_OF_PROGRAM{"TeSimplePlatformer"};
    enum
    {
        WINDOW_WIDTH = 40*32,
        WINDOW_HEIGHT = 12*32,
        TILE_SIZE = 32
    };

    std::unique_ptr<Player> mPlayer;
    sf::RenderWindow mWindow;
    sf::RectangleShape mBlock;
    sf::Texture mSpriteSet;

    //Should be replaced by MapManager
    std::vector<std::string> mMap
    {
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "X                                X     X",
        "X                                X     X",
        "X                                X     X",
        "X                                X     X",
        "X         0000               XXXXX     X",
        "X                                X     X",
        "XXX                              X     X",
        "X              XX                XXX   X",
        "X              XX                      X",
        "X    X         XX        XXX           X",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    };
    
    void inputPhase();
    void updatePhase(float time);
    void drawingPhase();
    void drawMap();
};

#endif /* GAME_HPP */

