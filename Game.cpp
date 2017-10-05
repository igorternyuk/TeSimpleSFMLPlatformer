/* 
 * File:   Game.cpp
 * Author: igor
 * 
 * Created on 30-09-2017 г., 12:11
 */

#include "Game.hpp"

#include <iostream>
#include <algorithm>

using components::CPosition;
using components::CPhysics;
using components::CAnimation;
using components::CPlayerControl;

Game::Game():
mWindow{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE_OF_PROGRAM}
{
    mWindow.setFramerateLimit(FPS);
    
    //Centralize main window
    
    int screenWidth = sf::VideoMode::getDesktopMode().width;
    int screenHeight = sf::VideoMode::getDesktopMode().height;
    mWindow.setPosition(sf::Vector2i((screenWidth - WINDOW_WIDTH) / 2,
                        (screenHeight - WINDOW_HEIGHT) / 2));
    
    //Load level data from text file
    
    //mLevel.loadFromFile("resources/levels/level.dat");
    mLevelManager.load(LevelID::FIRST, "resources/levels/level.dat");
    mCurrentLevel = &mLevelManager.get(LevelID::FIRST);
    mMap = mCurrentLevel->getMap();
    
    mTextureManager.load(TextureID::SPRITE_SET,
                         mCurrentLevel->getPathToSpriteSet());
    
    mTile.setTexture(mTextureManager.get(TextureID::SPRITE_SET)); // Это для рисования карты
       
    mFontManager.load(FontID::GERMAN, "resources/fonts/BRLNSR.TTF");
    
    mScoreLabel.setFont(mFontManager.get(FontID::GERMAN));
    mScoreLabel.setColor(sf::Color::Yellow);
    mScoreLabel.setCharacterSize(30);
    mScoreLabel.setPosition(5, 5);
   
    mStatusLabel.setFont(mFontManager.get(FontID::GERMAN));
    mStatusLabel.setColor(sf::Color::Green);
    mStatusLabel.setCharacterSize(80);
    

    loadSoundsAndMusic();
    
    //createEntities
    
    createEntities();

} 

Game::~Game() {
}

void Game::prepareNewGame()
{
    mManager.destroyAll();
    mScore = 0;
    //Reload map
    mMap = mCurrentLevel->getMap();
    createEntities();
    resetCamera();
    mStatus = GameStatus::PLAY;
    mState = GameState::PLAY;
}

void Game::createEntities()
{
    mPlayer = createPlayer();
    createEnemies();
}

void Game::togglePause()
{
    if(mState == GameState::PLAY)
        mState = GameState::PAUSE;
    else if(mState == GameState::PAUSE)
        mState = GameState::PLAY;
}

void Game::gameLoop()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    mBGM.play();
    ///////MAIN LOOP OF THE GAME //////////////////
    while (mWindow.isOpen())
    {
        processEvents();        
        timeSinceLastUpdate += clock.restart();
        while(timeSinceLastUpdate > mTimePerFrame)
        {
            timeSinceLastUpdate -= mTimePerFrame;
            processEvents();
            updatePhase(mTimePerFrame.asSeconds());
        }
        renderPhase();
    }
    mBGM.stop();
}

void Game::showSplashScreen()
{
    
}

void Game::showMenu()
{
    
}

void Game::render(const sf::Drawable& target)
{
    mWindow.draw(target);
}


ecs::Entity* Game::createPlayer()
{
    auto& entity(mManager.addEntity());
    //entity.addComponent<CPosition>(sf::Vector2f(100, 180));
    entity.addComponent<CPosition>(mCurrentLevel->getPlayerPos());
    
    int w{mCurrentLevel->getPlayerData()
    .bRectOnSpriteSet.width};
    int h{mCurrentLevel->getPlayerData()
    .bRectOnSpriteSet.height};
    entity.addComponent<CPhysics>(mCurrentLevel->getGravity(),sf::Vector2f(w,h));
    
    entity.addComponent<CPlayerControl>(mCurrentLevel->getPlayerData().moveSpeed,
            mCurrentLevel->getPlayerData().jumpVelocity);
    
    entity.addComponent<CAnimation>(this,
            mTextureManager.get(TextureID::SPRITE_SET),
            mCurrentLevel->getPlayerData()
            .bRectOnSpriteSet.left,
            mCurrentLevel->getPlayerData()
            .bRectOnSpriteSet.top,
            0.1f, mCurrentLevel->getPlayerData().numFrames,
            mCurrentLevel->getPlayerData().frameStep);
    
    auto& cPhysics(entity.getComponent<CPhysics>());    
    
    cPhysics.moveFunc = [&](float ft)
    {
        cPhysics.cPosition->pos.x += cPhysics.velocity.x * ft;
        
        //Check collision with respect to X-axis
        
        cPhysics.onCollision(true);
        
        if(!cPhysics.cPosition->isOnGround)
        {
            cPhysics.velocity.y += cPhysics.gravity * ft;
            cPhysics.cPosition->pos.y += cPhysics.velocity.y * ft;
        }      
        
        //Check collision with respect to Y-axis
        cPhysics.onCollision(false);
    };
    
    cPhysics.onCollision = [&](bool axisX)
    {
        int tileSize = mCurrentLevel->getTileSize();
            //Тут логика по обработке коллизий марио с картой
        for (int y = cPhysics.top()/ cPhysics.h();
                y < cPhysics.bottom() / cPhysics.h(); ++y)
        {
            for (int x = cPhysics.left() / cPhysics.w();
                    x < cPhysics.right() /cPhysics.w(); ++x)
            {
                if (mMap[y][x] == 'S' || mMap[y][x] == 'k' || mMap[y][x]=='0' ||
                    mMap[y][x]=='r' || mMap[y][x]=='t')
                {
                    if(axisX)
                    {
                        if (cPhysics.velocity.x > 0)
                        {
                            cPhysics.cPosition->pos.x = x * tileSize -
                                    cPhysics.w();
                        }
                        if (cPhysics.velocity.x < 0)
                        {
                            cPhysics.cPosition->pos.x = x * tileSize + tileSize;
                        }
                    }
                    else
                    {
                        if (cPhysics.velocity.y >= 0)
                        {
                            cPhysics.cPosition->pos.y = y * tileSize -
                                    cPhysics.h();
                            cPhysics.velocity.y = 0;
                            cPhysics.cPosition->isOnGround = true;
                        }
                        if (cPhysics.velocity.y < 0)
                        {
                            cPhysics.cPosition->pos.y = y * tileSize + tileSize;
                            cPhysics.velocity.y = 0;
                        }
                    }
                    break;
                }
                else if(mMap[y][x] == 'c')
                {
                    //Score
                    mMap[y][x] = ' ';
                    ++mScore;
                    mSounds[SoundID::COLLECT_POINT].play();
                }
                else
                {
                    cPhysics.cPosition->isOnGround = false;
                }
            }
        }
        checkWin();
    };
    
    auto& cPlayerControl(entity.getComponent<CPlayerControl>());
    
    cPlayerControl.controlFunc = [&]()
    {
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            cPlayerControl.cPhysics->velocity.x = 0;
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                cPlayerControl.cPhysics->velocity.x = -cPlayerControl.moveSpeed;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                cPlayerControl.cPhysics->velocity.x = cPlayerControl.moveSpeed;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            if (cPlayerControl.cPhysics->cPosition->isOnGround)
            {
                //Player can jump only from ground
                cPlayerControl.cPhysics->velocity.y = cPlayerControl.jumpVelocity;
                cPlayerControl.cPhysics->cPosition->isOnGround = false;
                mSounds[SoundID::JUMP].play();
            }
        }
    };
    
    entity.addGroup(GPlayers);
    return &entity;
 }

void Game::checkWin()
{
    if(mScore >= mCurrentLevel->getNumberOfPoints())
    {
        mStatus = GameStatus::VICTORY;
        mSounds[SoundID::VICTORY].play();
    }
}

ecs::Entity& Game::createEnemy(const sf::Vector2f& pos)
{
    auto& entity(mManager.addEntity());
    entity.addComponent<CPosition>(pos);
    int w{mCurrentLevel->getEnemyData()
    .bRectOnSpriteSet.width};
    int h{mCurrentLevel->getEnemyData()
    .bRectOnSpriteSet.height};
    entity.addComponent<CPhysics>(mCurrentLevel->getGravity(),sf::Vector2f(w,h));
    entity.addComponent<CAnimation>(this,
            mTextureManager.get(TextureID::SPRITE_SET),
            mCurrentLevel->getEnemyData()
            .bRectOnSpriteSet.left,
            mCurrentLevel->getEnemyData()
            .bRectOnSpriteSet.top,
            0.1f, mCurrentLevel->getEnemyData().numFrames,
            mCurrentLevel->getEnemyData().frameStep);
    
    auto& cPhysics(entity.getComponent<CPhysics>());    
    
    int randDir = mRandom.nextBool() ? 1 : -1;
    cPhysics.velocity.x = randDir * mCurrentLevel->getEnemyData().moveSpeed;
    
    cPhysics.moveFunc = [&cPhysics](float ft)
    {
        cPhysics.cPosition->pos.x += cPhysics.velocity.x * ft;
        //Check collision with respect to X-axis
        cPhysics.onCollision(true);
    };
    
    cPhysics.onCollision = [&](bool axisX)
    {
        //auto& map = mLevel.getMap();
        int tileSize = mCurrentLevel->getTileSize();
        
        //Enemy collisions logic
        
        for (int y = cPhysics.top() / cPhysics.h();
                y < cPhysics.bottom() / cPhysics.h(); ++y)
        {
            for (int x = cPhysics.left() / cPhysics.w();
                    x < cPhysics.right() /cPhysics.w(); ++x)
            {
                if (mMap[y][x] == 'S' || mMap[y][x]=='0' ||
                    mMap[y][x]=='r')
                {
                    if (axisX)
                    {
                        if (cPhysics.velocity.x > 0)
                        {
                            cPhysics.cPosition->pos.x = x * tileSize -
                                    cPhysics.w();
                        }
                        if (cPhysics.velocity.x < 0)
                        {
                            cPhysics.cPosition->pos.x = x * tileSize + tileSize;
                        }
                        cPhysics.velocity.x *= -1;
                    }
                    break;
                }
            }
        }
    };
    
    entity.addGroup(GEnemies);
    return entity;
}

void Game::createEnemies()
{
    for(const auto& pos: mCurrentLevel->getEnemyPositions())
        createEnemy(pos);
}

void Game::handleCollisionPE(ecs::Entity& player, ecs::Entity& enemy) noexcept
{
    auto& pPhycics(player.getComponent<CPhysics>());
    auto& ePhycics(enemy.getComponent<CPhysics>());
    
    if(isIntersecting(pPhycics, ePhycics))
    {
        if(pPhycics.cPosition->isOnGround)
        {
            mStatus = GameStatus::DEFEAT;
        }
        else
            enemy.destroy();            
    }
}

void Game::handleCollisionEE(ecs::Entity& enemy1, ecs::Entity& enemy2) noexcept
{
    auto& ePhycics1(enemy1.getComponent<CPhysics>());
    auto& ePhycics2(enemy2.getComponent<CPhysics>());
    
    if(!isIntersecting(ePhycics1, ePhycics2) || (&ePhycics1 == &ePhycics2))
        return;
    
    ePhycics1.velocity.x *= -1;
    ePhycics2.velocity.x *= -1;
}

void Game::processEvents()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            mWindow.close();
        }
        else if(event.type == sf::Event::KeyReleased)
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Space:
                togglePause();
                break;
            case sf::Keyboard::N:
                prepareNewGame();
                break;
            case sf::Keyboard::Escape:
                mWindow.close();
                break;
            default:
                break;
            }
        }
    }
}

void Game::updatePhase(float time)
{
    if(mState == GameState::PLAY && mStatus == GameStatus::PLAY)
    {
        mManager.refresh();
        mManager.update(time);

        auto& players(mManager.getEntitiesByGroup(GPlayers));
        auto& enemies(mManager.getEntitiesByGroup(GEnemies));

        for(auto& p: players)
            for(auto& e: enemies)
                handleCollisionPE(*p, *e);

        for(std::size_t i{0u}; i < enemies.size(); ++i)
        {
            for(std::size_t j{i + 1}; j < enemies.size(); ++j)
                handleCollisionEE(*enemies[i], *enemies[j]);
        }

        //Camera
        
        scrollCamera();
    }
}

void Game::renderPhase()
{
    mWindow.clear(sf::Color(107,140,255));
    drawMap();
    drawScore();
    mManager.draw();
    drawGameStatus();
    mWindow.display();
}


void Game::drawScore()
{
    mScoreLabel.setString(std::string("SCORE:") + std::to_string(mScore));
    mWindow.draw(mScoreLabel);
}

void Game::drawGameStatus()
{
    switch (mStatus)
    {
    case GameStatus::PLAY:
        if(mState == GameState::PAUSE)
        {
            mStatusLabel.setColor(sf::Color(72,0,255));
            mStatusLabel.setString("GAME PAUSED");
        }
        else if(mState == GameState::PLAY)
        {
            mStatusLabel.setColor(sf::Color::Transparent);
            mStatusLabel.setString("");
        }
        break;
    case GameStatus::VICTORY:
        mStatusLabel.setColor(sf::Color::Green);
        mStatusLabel.setString("YOU WON!!!");
        break;
    case GameStatus::DEFEAT:
        mStatusLabel.setColor(sf::Color::Red);
        mStatusLabel.setString("YOU LOST!!!");
        break;
    }
    int tw = mStatusLabel.getGlobalBounds().width;
    int th = mStatusLabel.getGlobalBounds().height;
    int lx = (WINDOW_WIDTH - tw) / 2;
    int ly = (WINDOW_HEIGHT - th) / 2 - th / 2;
    mStatusLabel.setPosition(lx, ly);
    mWindow.draw(mStatusLabel);
}

void Game::drawMap()
{
    for(std::size_t y{0u}; y < mMap.size(); ++y)
    {
        for (std::size_t x{0u}; x < mMap[y].size(); ++x)
        {
            if(mMap[y][x] == ' ' || mMap[y][x] == '0')
                continue;
            auto it = mCurrentLevel->getTiles()
            .find(mMap[y][x]);
            if(it != mCurrentLevel->getTiles().cend())
            {
                mTile.setTextureRect(it->second);
                mTile.setPosition(x * mCurrentLevel->getTileSize() - mCamera.x,
                y *  mCurrentLevel->getTileSize() - mCamera.y); 
                mWindow.draw(mTile);
            }
       }
    }
}

void Game::scrollCamera()
{
    if(mPlayer->getComponent<CPosition>().pos.x > WINDOW_WIDTH / 2 &&
       mPlayer->getComponent<CPosition>().pos.x <
            mMap[0].size() * mCurrentLevel->getTileSize() - WINDOW_WIDTH / 2)
    {
        mCamera.x = mPlayer->getComponent<CPosition>().pos.x - WINDOW_WIDTH / 2;
    }
    
    if(mPlayer->getComponent<CPosition>().pos.y > WINDOW_HEIGHT / 2 &&
        mPlayer->getComponent<CPosition>().pos.y <
            mMap.size() * mCurrentLevel->getTileSize() -
            WINDOW_HEIGHT / 2)
    {
        mCamera.y = mPlayer->getComponent<CPosition>().pos.y
                - WINDOW_HEIGHT / 2;
    }
}

void Game::resetCamera()
{
    mCamera.x = 0;
    mCamera.y = 0;
}

void Game::loadSoundsAndMusic()
{
    static const std::string PATH_TO_BGM{"resources/sounds/Mario_Theme.ogg"};
    if(!mBGM.openFromFile(PATH_TO_BGM))
        throw std::runtime_error("Could not load music file: " + PATH_TO_BGM);
    mBGM.setLoop(true);
    
    mSoundBufferManager.load(SoundID::JUMP, "resources/sounds/Jump.ogg");
    sf::Sound sound1(mSoundBufferManager.get(SoundID::JUMP));
    mSounds.insert({SoundID::JUMP, sound1});
    
    mSoundBufferManager.load(SoundID::COLLECT_POINT,
                             "resources/sounds/CollectPoint.wav");
    sf::Sound sound2(mSoundBufferManager.get(SoundID::COLLECT_POINT));
    mSounds.insert({SoundID::COLLECT_POINT, sound2});
    
    mSoundBufferManager.load(SoundID::VICTORY, "resources/sounds/Victory.wav");
    sf::Sound sound3(mSoundBufferManager.get(SoundID::VICTORY));
    mSounds.insert({SoundID::VICTORY, sound3});
    
    mSoundBufferManager.load(SoundID::DEFEAT, "resources/sounds/Defeat.wav");
    sf::Sound sound4(mSoundBufferManager.get(SoundID::DEFEAT));
    mSounds.insert({SoundID::DEFEAT, sound4});
}
