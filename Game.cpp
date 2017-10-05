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
    mLevel.loadFromFile("resources/levels/level.dat");
    mTextureManager.load(TextureID::SPRITE_SET, mLevel.getPathToSpriteSet());
    mTile.setTexture(mTextureManager.get(TextureID::SPRITE_SET)); // Это для рисования карты
    
    mPlayer = createPlayer();
    createEnemies();
   
    mFontManager.load(FontID::GERMAN, "resources/fonts/BRLNSR.TTF");
    
    mScoreLabel.setFont(mFontManager.get(FontID::GERMAN));
    mScoreLabel.setColor(sf::Color::Yellow);
    mScoreLabel.setCharacterSize(30);
    mScoreLabel.setPosition(5, 5);
   
    mStatusLabel.setFont(mFontManager.get(FontID::GERMAN));
    mStatusLabel.setColor(sf::Color::Green);
    mStatusLabel.setCharacterSize(80);
    
    mBGM.openFromFile("resources/sounds/Mario_Theme.ogg");
    mBGM.setLoop(true);
    loadSounds();
    
    auto& enemies(mManager.getEntitiesByGroup(GEnemies));
    std::cout << "enemies.size() = " << enemies.size() << std::endl;
} 

Game::~Game() {
}

void Game::render(const sf::Drawable& target)
{
    mWindow.draw(target);
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

ecs::Entity* Game::createPlayer()
{
    auto& entity(mManager.addEntity());
    entity.addComponent<CPosition>(mLevel.getPlayerPos());
    int w{mLevel.getPlayerData().bRectOnSpriteSet.width};
    int h{mLevel.getPlayerData().bRectOnSpriteSet.height};
    entity.addComponent<CPhysics>(mLevel.getGravity(),sf::Vector2f(w,h));
    entity.addComponent<CPlayerControl>(mLevel.getPlayerData().moveSpeed,
            mLevel.getPlayerData().jumpVelocity);
    entity.addComponent<CAnimation>(this,
            mTextureManager.get(TextureID::SPRITE_SET),
            mLevel.getPlayerData().bRectOnSpriteSet.left,
            mLevel.getPlayerData().bRectOnSpriteSet.top,
            0.1f, mLevel.getPlayerData().numFrames,
            mLevel.getPlayerData().frameStep);
    
    auto& cPhysics(entity.getComponent<CPhysics>());    
    
    cPhysics.moveFunc = [&cPhysics](float ft)
    {
        cPhysics.cPosition->pos.x += cPhysics.velocity.x * ft;
        //Check collision with respect to X-axis
        cPhysics.onCollision(true);
        if(!cPhysics.cPosition->isOnGround)
        {
            cPhysics.velocity.y += cPhysics.gravity * ft;
        }
        cPhysics.cPosition->pos.y += cPhysics.velocity.y * ft;
        cPhysics.cPosition->isOnGround = false;
        //Check collision with respect to Y-axis
        cPhysics.onCollision(false);
    };
    
    cPhysics.onCollision = [&](bool axisX)
    {
        auto& map = mLevel.getMap();
        int tileSize = mLevel.getTileSize();
            //Тут логика по обработке коллизий марио с картой
        for (int y = cPhysics.top() / cPhysics.h();
                y < cPhysics.bottom() / cPhysics.h(); ++y)
        {
            for (int x = cPhysics.left() / cPhysics.w();
                    x < cPhysics.right() /cPhysics.w(); ++x)
            {
                if (map[y][x] == 'S' || map[y][x] == 'k' || map[y][x]=='0' ||
                    map[y][x]=='r' || map[y][x]=='t')
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
                else if(map[y][x] == 'c')
                {
                    //Score
                    map[y][x] = ' ';
                    ++mScore;
                    mSounds[SoundID::COLLECT_POINT].play();
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
                cPlayerControl.cPhysics->velocity.y = 
                        cPlayerControl.jumpVelocity;
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
    if(mScore >= mLevel.getNumberOfPoints())
    {
        mStatus = GameStatus::VICTORY;
        mSounds[SoundID::VICTORY].play();
    }
}

ecs::Entity& Game::createEnemy(const sf::Vector2f& pos)
{
    auto& entity(mManager.addEntity());
    entity.addComponent<CPosition>(pos);
    int w{mLevel.getEnemyData().bRectOnSpriteSet.width};
    int h{mLevel.getEnemyData().bRectOnSpriteSet.height};
    entity.addComponent<CPhysics>(mLevel.getGravity(),sf::Vector2f(w,h));
    entity.addComponent<CAnimation>(this,
            mTextureManager.get(TextureID::SPRITE_SET),
            mLevel.getEnemyData().bRectOnSpriteSet.left,
            mLevel.getEnemyData().bRectOnSpriteSet.top,
            0.1f, mLevel.getEnemyData().numFrames,
            mLevel.getEnemyData().frameStep);
    
    auto& cPhysics(entity.getComponent<CPhysics>());    
    
    int randDir = mRandom.nextBool() ? 1 : -1;
    cPhysics.velocity.x = randDir * mLevel.getEnemyData().moveSpeed;
    
    cPhysics.moveFunc = [&cPhysics](float ft)
    {
        cPhysics.cPosition->pos.x += cPhysics.velocity.x * ft;
        //Check collision with respect to X-axis
        cPhysics.onCollision(true);
    };
    
    cPhysics.onCollision = [&](bool axisX)
    {
        auto& map = mLevel.getMap();
        int tileSize = mLevel.getTileSize();
        
        //Enemy collisions logic
        
        for (int y = cPhysics.top() / cPhysics.h();
                y < cPhysics.bottom() / cPhysics.h(); ++y)
        {
            for (int x = cPhysics.left() / cPhysics.w();
                    x < cPhysics.right() /cPhysics.w(); ++x)
            {
                if (map[y][x] == 'S' || map[y][x]=='0' ||
                    map[y][x]=='r')
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
    std::cout << "numEnemies = " << mLevel.getEnemyPositions().size() <<
            std::endl;
    for(const auto& pos: mLevel.getEnemyPositions())
        createEnemy(pos);
}

void Game::handleCollisionPE(ecs::Entity& player, ecs::Entity& enemy) noexcept
{
    auto& pPhycics(player.getComponent<CPhysics>());
    auto& ePhycics(enemy.getComponent<CPhysics>());
    
    if(isIntersecting(pPhycics, ePhycics))
    {
        if(pPhycics.cPosition->isOnGround)
            mStatus = GameStatus::DEFEAT;
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
    }
}

void Game::updatePhase(float time)
{
    mManager.refresh();
    mManager.update(time);
    
    auto& players(mManager.getEntitiesByGroup(GPlayers));
    auto& enemies(mManager.getEntitiesByGroup(GEnemies));
    
    for(auto& p: players)
    {
        for(auto& e: enemies)
        {
            handleCollisionPE(*p, *e);
        }
    }
    
   /* for(auto& e1: enemies)
    {
        for(auto& e2: enemies)
        {
            if(e1 != e2)
                handleCollisionEE(*e1, *e2);
        }
    }*/

    for(int i = 0; i < enemies.size(); ++i)
    {
        for(int j = i + 1; j < enemies.size(); ++j)
            handleCollisionEE(*enemies[i], *enemies[j]);
    }
    
    //Camera
    scrollCamera();
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
        mStatusLabel.setString("");
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
    int tx = (WINDOW_WIDTH - mStatusLabel.getGlobalBounds().width) / 2;
    int ty = (WINDOW_HEIGHT - mStatusLabel.getGlobalBounds().height) / 2;
    mStatusLabel.setPosition(tx, ty);
    mWindow.draw(mStatusLabel);
}

void Game::drawMap()
{
    //Прорисовка карты
    auto& map = mLevel.getMap();
    for(std::size_t y{0u}; y < map.size(); ++y)
    {
        for (std::size_t x{0u}; x < map[y].size(); ++x)
        {
            if(map[y][x] == ' ' || map[y][x] == '0')
                continue;
            auto it = mLevel.getTiles().find(map[y][x]);
            if(it != mLevel.getTiles().cend())
            {
                mTile.setTextureRect(it->second);
                mTile.setPosition(x * mLevel.getTileSize() - mCamera.x,
                                  y *  mLevel.getTileSize() - mCamera.y); 
                mWindow.draw(mTile);
            }
       }
    }
}

void Game::scrollCamera()
{
    if(mPlayer->getComponent<CPosition>().pos.x > WINDOW_WIDTH / 2 &&
       mPlayer->getComponent<CPosition>().pos.x <
            mLevel.getMap()[0].size() * mLevel.getTileSize() - WINDOW_WIDTH / 2)
    {
        mCamera.x = mPlayer->getComponent<CPosition>().pos.x - WINDOW_WIDTH / 2;
    }
    if(mPlayer->getComponent<CPosition>().pos.y > WINDOW_HEIGHT / 2 &&
        mPlayer->getComponent<CPosition>().pos.y <
            mLevel.getMap().size() * mLevel.getTileSize() - WINDOW_HEIGHT / 2)
    {
        mCamera.y = mPlayer->getComponent<CPosition>().pos.y
                - WINDOW_HEIGHT / 2;
    }
}

void Game::loadSounds()
{
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
