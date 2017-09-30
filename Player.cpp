/* 
 * File:   Player.cpp
 * Author: igor
 * 
 * Created on 30-09-2017., 11:27
 */

#include "Player.hpp"
#include <iostream>

Player::Player(int tileSize) :
mVelX{0}, mVelY{0}, mCurrentFrame{0}, mBoundingRect{40, 200, 40, 50},
mTileSize{tileSize}
{
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
    mSprite.setTextureRect({0, 244, 40, 50});
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

void Player::update(float time, const std::vector<std::string> &map)
{
    mBoundingRect.left += mVelX * time;
    handleCollision(Direction::X, map);
    handleCollision(Direction::Y, map);
    if (!mIsOnGround)
    {
        mVelY += GRAVITY * time;
    }
    mBoundingRect.top += mVelY * time;

    //Animation
    mCurrentFrame += ANIMATION_SPEED * time;
    if (mCurrentFrame > 6) mCurrentFrame = 0;
    mSprite.move(mVelX * time, 0);
    if (mVelX > 0)
        mSprite.setTextureRect({40 * int(mCurrentFrame), 244, 40, 50});
    if (mVelX < 0)
        mSprite.setTextureRect({40 * int(mCurrentFrame) + 40, 244, -40, 50});
    //If player touches ground
    if (!mIsOnGround && mBoundingRect.top + mBoundingRect.height >= 600)
    {
        std::cout << "is on ground = " << mIsOnGround << std::endl;
        std::cout << "Player has touch the ground";
        mIsOnGround = true;
        std::cout << "is on ground = " << mIsOnGround << std::endl;
        mVelY = 0;
        mBoundingRect.top = 12 * mTileSize - mBoundingRect.height;
    }
    mSprite.setPosition(mBoundingRect.left, mBoundingRect.top);
    mVelX = 0;
}

void Player::draw(sf::RenderWindow &window) const
{
    window.draw(mSprite);
}

void Player::handleCollision(Direction dir, const std::vector<std::string> &map)
{
    std::cout << "Handle collisions map.size = " << map.size() << std::endl;
    int tx = (mBoundingRect.left + mBoundingRect.width) / mTileSize;
    int ty = (mBoundingRect.top + mBoundingRect.height) / mTileSize;
    std::cout << "tx = " << tx << " ty = " << ty << std::endl;
    for (int y = mBoundingRect.top / mTileSize; y < ty; ++y)
    {
        for (int x = mBoundingRect.top / mTileSize; x < tx; ++x)
        {
            if (map[y][x] == 'X')
            {
                std::cout << "CollisionX" << std::endl;
                if (dir == Direction::X)
                {
                    if (mVelY > 0)
                    {
                        mBoundingRect.left = x * mTileSize - mBoundingRect.width;
                        mIsOnGround = true;
                    }
                    if (mVelY < 0)
                    {
                        mBoundingRect.left = x * mTileSize + mTileSize;
                    }
                }
                else
                {
                    if (mVelY > 0)
                    {
                        mBoundingRect.left = y * mTileSize - mBoundingRect.height;
                    }
                    if (mVelY < 0)
                    {
                        mBoundingRect.top = y * mTileSize + mTileSize;
                    }
                }
            }
        }
    }
}