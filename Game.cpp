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
using components::CStaticImage;
using components::CPlayerControl;

Game::Game():
mWindow{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE_OF_PROGRAM}
{
    //mTextureManager.get(TextureID::SPRITE_SET).loadFromFile("resources/gfx/MarioTileSet.png");
    mLevel.loadFromFile("resources/levels/level.dat");
    mTextureManager.load(TextureID::SPRITE_SET, mLevel.getPathToSpriteSet());
    mTile.setTexture(mTextureManager.get(TextureID::SPRITE_SET)); // Это для рисования карты
    mPlayer = createPlayer();
    mFontManager.load(FontID::GERMAN, "resources/fonts/BRLNSR.TTF");
    mScoreLabel.setFont(mFontManager.get(FontID::GERMAN));
    mScoreLabel.setColor(sf::Color::Yellow);
    mScoreLabel.setCharacterSize(30);
    mScoreLabel.setPosition(5, 5);
    mSoundBufferManager.load(SoundBuffertID::JUMP, "resources/sounds/Jump.ogg");
    mSoundJump.setBuffer( mSoundBufferManager.get(SoundBuffertID::JUMP));
    //mTextureManager.get(TextureID::SPRITE_SET);
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
    entity.addComponent<CAnimation>(this, mTextureManager.get(TextureID::SPRITE_SET),
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
                        cPhysics.cPosition->pos.x = x * tileSize - cPhysics.w();
                    }
                    if (cPhysics.velocity.x < 0)
                    {
                        cPhysics.cPosition->pos.x = x * tileSize + tileSize;
                    }
                }
                else
                {
                    if (cPhysics.velocity.y > 0)
                    {
                        cPhysics.cPosition->pos.y = y * tileSize - cPhysics.h();
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
            }
        }
    }
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
            //std::cout << "jump" << std::endl;
            if (cPlayerControl.cPhysics->cPosition->isOnGround)
            {
                //Player can jump only from ground
                cPlayerControl.cPhysics->velocity.y = 
                        cPlayerControl.jumpVelocity;
                cPlayerControl.cPhysics->cPosition->isOnGround = false;
                mSoundJump.play();
            }
        }
    };
    
    entity.addGroup(GPlayers);
    return &entity;
 }

ecs::Entity& Game::createEnemy()
{
    
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
    //Camera
    scrollCamera();
}

void Game::renderPhase()
{
    mWindow.clear(sf::Color(107,140,255));
    drawMap();
    drawScore();
    mManager.draw();
    mWindow.display();
}


void Game::drawScore()
{
    mScoreLabel.setString(std::string("SCORE:") + std::to_string(mScore));
    mWindow.draw(mScoreLabel);
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
