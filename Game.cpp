/* 
 * File:   Game.cpp
 * Author: igor
 * 
 * Created on 30-09-2017 г., 12:11
 */

#include "Game.hpp"
#include <iostream>

Game::Game():
mPlayer{std::make_unique<Player>()},
mWindow{sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE_OF_PROGRAM},
mBlock(sf::Vector2f(TILE_SIZE,TILE_SIZE))
{
    int px = 7 * 32, py = 9 * 32;
    mSpriteSet.loadFromFile("resources/gfx/MarioTileSet.png");
    auto upPlayer = std::make_unique<Player>(mSpriteSet, sf::IntRect(
    PLAYER_POS_ON_SPRITE_X, PLAYER_POS_ON_SPRITE_Y, PLAYER_WIDTH, PLAYER_HEIGHT),
    PLAYER_VELOCITY, PLAYER_JUMP_VELOCITY, 32);
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

ecs::Entity& Game::createPlayer()
{
    /*            auto& entity(manager.addEntity());

            entity.addComponent<CPosition>(
                Vector2f{windowWidth / 2.f, windowHeight / 2.f});
            entity.addComponent<CPhysics>(Vector2f{ballRadius, ballRadius});
            entity.addComponent<CCircle>(this, ballRadius);

            auto& cPhysics(entity.getComponent<CPhysics>());
            cPhysics.velocity = Vector2f{-ballVelocity, -ballVelocity};
            cPhysics.onOutOfBounds = [&cPhysics](const Vector2f& mSide)
            {
                if(mSide.x != 0.f)
                    cPhysics.velocity.x =
                        std::abs(cPhysics.velocity.x) * mSide.x;

                if(mSide.y != 0.f)
                    cPhysics.velocity.y =
                        std::abs(cPhysics.velocity.y) * mSide.y;
            };

            entity.addGroup(ArkanoidGroup::GBall);

            return entity;*/
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
    mPlayer->handleUserInput();
}

void Game::updatePhase(float time)
{
    mPlayer->update(time, mMap);
    //Camera
    scrollCamera();
}

void Game::renderPhase()
{
    mWindow.clear(sf::Color::Cyan);
    drawMap();
    mPlayer->draw(mWindow, mCamera.x, mCamera.y);
    mWindow.display();
}

void Game::drawMap()
{
    //Прорисовка карты
    /*		 for (int i=0; i<H; i++)
			 for (int j=0; j<W; j++)
			  { 
				if (TileMap[i][j]=='P')  tile.setTextureRect( IntRect(143-16*3,112,16,16) );

				if (TileMap[i][j]=='k')  tile.setTextureRect( IntRect(143,112,16,16) );
				                                 
   			    if (TileMap[i][j]=='c')  tile.setTextureRect( IntRect(143-16,112,16,16) );

				if (TileMap[i][j]=='t')  tile.setTextureRect( IntRect(0,47,32,95-47) );

				if (TileMap[i][j]=='g')  tile.setTextureRect( IntRect(0,16*9-5,3*16,16*2+5) );

				if (TileMap[i][j]=='G')  tile.setTextureRect( IntRect(145,222,222-145,255-222) );

			    if (TileMap[i][j]=='d')  tile.setTextureRect( IntRect(0,106,74,127-106) );

				if (TileMap[i][j]=='w')  tile.setTextureRect( IntRect(99,224,140-99,255-224) );

				if (TileMap[i][j]=='r')  tile.setTextureRect( IntRect(143-32,112,16,16) );

				if ((TileMap[i][j]==' ') || (TileMap[i][j]=='0')) continue;

  			    tile.setPosition(j*16-offsetX,i*16 - offsetY) ; 
		        window.draw(tile);
		     }
		 */
    for (std::size_t y = 0; y < mMap.size(); ++y)
    {
        for (std::size_t x = 0; x < mMap[y].size(); ++x)
        {
            if(mMap[y][x] == 'X')
            {
                mBlock.setFillColor(sf::Color(127, 0, 0));
            }
            else if(mMap[y][x] == '0')
            {
                mBlock.setFillColor(sf::Color(127, 127, 0));
            }
            else
                continue;
            mBlock.setPosition(x * TILE_SIZE - mCamera.x, y * TILE_SIZE - mCamera.y);
            mWindow.draw(mBlock);
        }
    }
    
    /**/
}

void Game::scrollCamera()
{
    if(mPlayer->getX() > WINDOW_WIDTH / 2 &&
            mPlayer->getX() < mMap[0].size() * TILE_SIZE - WINDOW_WIDTH / 2 )
    {
        mCamera.x = mPlayer->getX() - WINDOW_WIDTH / 2;
    }
    if(mPlayer->getY() > WINDOW_HEIGHT / 2 &&
            mPlayer->getY() < mMap.size() * TILE_SIZE - WINDOW_HEIGHT / 2 )
    {
        mCamera.y = mPlayer->getY() - WINDOW_HEIGHT / 2;
    }
}
