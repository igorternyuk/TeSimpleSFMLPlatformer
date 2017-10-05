/* 
 * File:   Game.hpp
 * Author: igor
 *
 * Created on 30-09-2017 г., 12:11
 */

#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Audio.hpp>
#include "random.h"
#include "ResourceManager.hpp"
#include "composition.hpp"
#include "components/CPosition.hpp"
#include "components/CPhysics.hpp"
#include "components/CAnimation.hpp"
#include "components/CPlayerControl.hpp"
#include "Level.hpp"
#include <vector>
#include <string>
#include <memory>

class Game {
public:
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
    
    explicit Game();
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
    virtual ~Game();
    void gameLoop();
    void render(const sf::Drawable& target);
    inline auto getStatus() const noexcept { return mStatus; }
    inline auto getState() const noexcept { return mState; }
    inline auto getCameraX() const noexcept { return mCamera.x; }
    inline auto getCameraY() const noexcept { return mCamera.y; }
    
private:    

    enum { WINDOW_WIDTH = 800, WINDOW_HEIGHT = 256, FPS = 60 };    
    enum PlatformerGroup : std::size_t { GPlayers, GEnemies };    
    const std::string TITLE_OF_PROGRAM{"TeSimplePlatformer"};
    Level mLevel;
    const sf::Time mTimePerFrame{sf::seconds(1.0f / FPS)};
    sf::RenderWindow mWindow;
    sf::Sprite mTile;
    ecs::EntityManager mManager;    
    ecs::Entity* mPlayer;
    Random mRandom;
    sf::Vector2f mCamera;
    GameState mState = GameState::PLAY;
    GameStatus mStatus = GameStatus::PLAY;
    sf::Text mStatusLabel;
    
    //Textures
    
    enum class TextureID { SPRITE_SET = 0 };
    ResourceManager<TextureID, sf::Texture> mTextureManager;
    
    //Fonst
    
    enum class FontID { GERMAN = 0 };
    ResourceManager<FontID, sf::Font> mFontManager;
    
    //Sounds and music
    
    enum class SoundID
    { 
        JUMP,
        COLLECT_POINT,
        VICTORY,
        DEFEAT
    };
    ResourceManager<SoundID, sf::SoundBuffer> mSoundBufferManager;
    std::map<SoundID, sf::Sound> mSounds;
    sf::Music mBGM;
   
    int mScore{0};
    sf::Text mScoreLabel;
    
    void showSplashScreen();
    void showMenu();
    
    template <typename T1, typename T2>
    bool isIntersecting(const T1& obj1, const T2& obj2)
    {
        return !(obj1.right() < obj2.left() || obj1.left() > obj2.right() ||
               obj1.bottom() < obj2.top() || obj1.top() > obj2.bottom());
    }   

    //Simple "factory" functions
    
    ecs::Entity* createPlayer();
    ecs::Entity& createEnemy(const sf::Vector2f& pos);
    void createEnemies();
    
    //Collsions
    
    void handleCollisionPE(ecs::Entity& player, ecs::Entity& enemy) noexcept;
    void handleCollisionEE(ecs::Entity& enemy1, ecs::Entity& enemy2) noexcept;
    
    void checkWin();
    void prepareNewGame();
    void processEvents();
    void updatePhase(float time);
    void renderPhase();
    void drawMap();
    void drawScore();
    void drawGameStatus();
    void scrollCamera();
    void loadSounds();
};

#endif /* GAME_HPP */

