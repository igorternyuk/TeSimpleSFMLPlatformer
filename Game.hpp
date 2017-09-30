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

/**/
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
        WINDOW_WIDTH = 800,
        WINDOW_HEIGHT = 600,
        TILE_SIZE = 32
    };
    const float FPS{60.f};
    const sf::Time mTimePerFrame{sf::seconds(1 / FPS)};
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
        "X                                X     X",
        "X                                X     X",
        "XXXXXXXX                         X     X",
        "X                                X     X",
        "X                                X     X",
        "X                                X     X",
        "X                                X     X",
        "X                                X     X",
        "X                                X    0X",
        "X                             0  X     X",
        "X         0000               XXXXX     X",
        "X 0                              X     X",
        "XXX             0                X 0   X",
        "X              XX                XXX   X",
        "X    0         XX         0            X",
        "X    X         XX        XXX           X",
        "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
    };
    float offsetX = 0;
    float offsetY = 0;
    void processEvents();
    void updatePhase(float time);
    void renderPhase();
    void drawMap();
};

#endif /* GAME_HPP */

