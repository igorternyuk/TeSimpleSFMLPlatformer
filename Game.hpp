/* 
 * File:   Game.hpp
 * Author: igor
 *
 * Created on 30-09-2017 г., 12:11
 */

#ifndef GAME_HPP
#define GAME_HPP

#include "composition.hpp"
#include "components/CPosition.hpp"
#include "components/CPhysics.hpp"
#include "components/CAnimation.hpp"
#include "components/CPlayerControl.hpp"
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
    virtual ~Game();
    void gameLoop();
    void render();
    inline auto getCameraX() const noexcept { return mCamera.x; }
    inline auto getCameraY() const noexcept { return mCamera.y; }
private:
    enum class GameState
    {
        UNINITIALIZED,
        SPLASH_SCREEN,
        MENU,
        PLAY,
        PAUSE,
        EXIT
    };
    enum class GameStatus
    {
        PLAY,
        VICTORY,
        DEFEAT
    };
    
    enum
    {
        WINDOW_WIDTH = 800,
        WINDOW_HEIGHT = 272,
        TILE_SIZE = 16,
        PLAYER_POS_ON_SPRITE_X = 0,
        PLAYER_POS_ON_SPRITE_Y = 244,
        PLAYER_VELOCITY = 90,
        PLAYER_JUMP_VELOCITY = -230
    };
    
    enum PlatformerGroup : std::size_t
    {
        GPlayers,
        GEnamies
    };
    const std::string TITLE_OF_PROGRAM{"TeSimplePlatformer"};
    const float FPS{60.f};
    const sf::Time mTimePerFrame{sf::seconds(1 / FPS)};
    sf::RenderWindow mWindow;
    sf::RectangleShape mBlock;
    sf::Texture mSpriteSet;
    ecs::EntityManager mManager;
    
    std::unique_ptr<Player> mPlayer;
    sf::Vector2f mCamera;

    //Should be replaced by MapManager
    std::vector<std::string> mMap
    {
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
        "0                                                                                                                                                    0",
        "0                                                                                    w                                                               0",
        "0                   w                                  w                   w                                                                         0",
        "0                                      w                                       kk                                                                    0",
        "0                                                                             k  k    k    k                                                         0",
        "0                      c                                                      k      kkk  kkk  w                                                     0",
        "0                                                                       r     k       k    k                                                         0",
        "0                                                                      rr     k  k                                                                   0",
        "0                                                                     rrr      kk                                                                    0",
        "0               c    kckck                                           rrrr                                                                            0",
        "0                                      t0                           rrrrr                                                                            0",
        "0G                                     00              t0          rrrrrr            G                                                               0",
        "0           d    g       d             00              00         rrrrrrr                                                                            0",
        "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
        "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
}; 
    void showSplashScreen();
    void showMenu();
    
    template <typename T1, typename T2>
    bool isIntersecting(const T1& obj1, const T2& obj2)
    {
        return obj1.right() >= obj2.left() && obj1.left() <= obj2.right() &&
               obj1.bottom() >= obj2.top() && obj1.top() <= obj2.bottom();
    }
    //Тут надо проработать столкновения игрока с врагами и врагов между собой
    /*   
    void testCollisionPB(Entity& mPaddle, Entity& mBall) noexcept
    {
        auto& cpPaddle(mPaddle.getComponent<CPhysics>());
        auto& cpBall(mBall.getComponent<CPhysics>());

        if(!isIntersecting(cpPaddle, cpBall)) return;

        cpBall.velocity.y = -ballVelocity;
        if(cpBall.x() < cpPaddle.x())
            cpBall.velocity.x = -ballVelocity;
        else
            cpBall.velocity.x = ballVelocity;
    }*/
    //Фабричные функции
    ecs::Entity& createPlayer();
    ecs::Entity& createEnemy();
    void processEvents();
    void updatePhase(float time);
    void renderPhase();
    void drawMap();
    void scrollCamera();
};

#endif /* GAME_HPP */

