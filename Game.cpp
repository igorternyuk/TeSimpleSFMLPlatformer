/* 
 * File:   Game.cpp
 * Author: igor
 * 
 * Created on 30-09-2017 г., 12:11
 */

#include "Game.hpp"

#include <iostream>

using components::CPosition;
using components::CPhysics;
using components::CAnimation;
using components::CStaticImage;
using components::CPlayerControl;

Game::Game():
mWindow{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE_OF_PROGRAM}
{
    mSpriteSet.loadFromFile("resources/gfx/MarioTileSet.png");
    mLevel.loadFromFile("resources/levels/level.dat");
    mTile.setTexture(mSpriteSet); // Это для рисования карты
    mPlayer = createPlayer();
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
    entity.addComponent<CAnimation>(this, mSpriteSet,
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
    for (int y = cPhysics.top() / cPhysics.h(); y < cPhysics.bottom() / cPhysics.h(); ++y)
    {
        for (int x = cPhysics.left() / cPhysics.w(); x < cPhysics.right() /cPhysics.w(); ++x)
        {
            //(TileMap[i][j]=='P') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='t')
            if (map[y][x] == 'S' || map[y][x] == 'k' || map[y][x]=='0' || map[y][x]=='r' || map[y][x]=='t')
            {
                if (axisX)
                {
                    if (cPhysics.velocity.x > 0)
                    {
                        cPhysics.cPosition->pos.x = x * tileSize - cPhysics.w();
                    }
                        //mBoundingRect.left = x * mTileSize - mBoundingRect.width;
                    if (cPhysics.velocity.x < 0)
                    {
                        cPhysics.cPosition->pos.x = x * tileSize + tileSize;
                    }
                        //mBoundingRect.left = x * mTileSize + mTileSize;
                }
                else
                {
                    if (cPhysics.velocity.y > 0)
                    {
                        cPhysics.cPosition->pos.y = y * tileSize - cPhysics.h();
                        //mBoundingRect.top = y * mTileSize - h();
                        cPhysics.velocity.y = 0;
                        cPhysics.cPosition->isOnGround = true;
                    }
                    if (cPhysics.velocity.y < 0)
                    {
                        cPhysics.cPosition->pos.y = y * tileSize + tileSize;
                        //mBoundingRect.top = y * mTileSize + mTileSize;
                        cPhysics.velocity.y = 0;
                    }
                }
                break;
            }
            else if(map[y][x] == '0')
            {
                map[y][x] = ' ';
            }
        }
    }
    };
    
    auto& cPlayerControl(entity.getComponent<CPlayerControl>());
    
    cPlayerControl.controlFunc = [&cPlayerControl]()
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
                cPlayerControl.cPhysics->velocity.y = cPlayerControl.jumpVelocity;
                cPlayerControl.cPhysics->cPosition->isOnGround = false;
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
    mManager.draw();
    mWindow.display();
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
        mCamera.y = mPlayer->getComponent<CPosition>().pos.y - WINDOW_HEIGHT / 2;
    }
}
