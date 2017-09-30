/* 
 * File:   Player.cpp
 * Author: igor
 * 
 * Created on 30-09-2017., 11:27
 */

#include "Player.hpp"
#include <iostream>

Player::Player(int tileSize) :
mVelX{0}, mVelY{0}, mCurrentFrame{0}, mBoundingRect{2 * 32,6 * 32, 40, 50},
mTileSize{tileSize}
{
    mSprite.setPosition(7 * 32, 9 * 32);
}

Player::Player(sf::Texture &image, int tileSize) : Player(tileSize)
{
    mSprite.setTexture(image);
}

Player::~Player()
{
}

void Player::setTexture(sf::Texture &image)
{
    mSprite.setTexture(image);
    mSprite.setTextureRect({0, 244, mBoundingRect.width, mBoundingRect.height});
}

void Player::handleUserInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        mVelX = -VELOCITY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        mVelX = VELOCITY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {

    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //std::cout << "jump" << std::endl;
        if (mIsOnGround)
        {
            //Player can jump only from ground
            mVelY = JUMP_VELOCITY;
            mIsOnGround = false;
        }
    }
}

void Player::update(float time, std::vector<std::string> &map)
{
    mBoundingRect.left += mVelX * time;
    handleCollision(true, map);

    if (!mIsOnGround)
    {
        mVelY += GRAVITY * time;
    }

    mBoundingRect.top += mVelY * time;
    
    mIsOnGround = false;
    handleCollision(false, map);

    //Animation

    mCurrentFrame += ANIMATION_SPEED * time;
    if (mCurrentFrame > 6) mCurrentFrame = 0;
    if(mVelX == 0)
    {
        mCurrentFrame = 0;
        if(isLastDirRight)
            mSprite.setTextureRect({40 * int(mCurrentFrame), 244, 40, 50});
        else
            mSprite.setTextureRect({40 * int(mCurrentFrame) + 40, 244, -40, 50});
    } 
    else if (mVelX > 0)
    {
        mSprite.setTextureRect({40 * int(mCurrentFrame), 244, 40, 50});
        isLastDirRight = true;
    }
    else
    {
        mSprite.setTextureRect({40 * int(mCurrentFrame) + 40, 244, -40, 50});
        isLastDirRight = false;
    }
    //If player touches ground

    mVelX = 0;
}

void Player::draw(sf::RenderWindow &window, float offsetX, float offsetY)
{    
    mSprite.setPosition(mBoundingRect.left - offsetX, mBoundingRect.top - offsetY);
    window.draw(mSprite);
}

void Player::handleCollision(bool isAxisX, std::vector<std::string> &map)
{
    for (int y = mBoundingRect.top / mTileSize;
            y < (mBoundingRect.top + mBoundingRect.height) / mTileSize; ++y)
    {
        for (int x = mBoundingRect.left / mTileSize;
                x < (mBoundingRect.left + mBoundingRect.width) / mTileSize; ++x)
        {
            std::cout << "x = " << x << " y = " << y << std::endl;
            std::cout << "map[y][x] == 'X' - > " << (map[y][x] == 'X') << std::endl;
            if (map[y][x] == 'X')
            {
                if (isAxisX)
                {
                    if (mVelX > 0)
                        mBoundingRect.left = x * mTileSize - mBoundingRect.width;
                    if (mVelX < 0)
                        mBoundingRect.left = x * mTileSize + mTileSize;
                }
                else
                {
                    if (mVelY > 0)
                    {
                        mBoundingRect.top = y * mTileSize - mBoundingRect.height;
                        mVelY = 0;
                        mIsOnGround = true;
                    }
                    if (mVelY < 0)
                    {
                        mBoundingRect.top = y * mTileSize + mTileSize;
                        mVelY = 0;
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
}