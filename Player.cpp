/* 
 * File:   Player.cpp
 * Author: igor
 * 
 * Created on 30-09-2017., 11:27
 */

#include "Player.hpp"
#include <iostream>

/*  sf::Sprite mSprite;
    float mVelocity;
    float mJumpVelocity;
    int mTileSize*/

Player::Player(sf::Texture &image, const sf::IntRect &tRect,
               float velocity, float jumpVelocity, int posOnSpriteSetX = 0,
               int posOnSpriteSetY = 0, int tileSize = 32):
mSprite(image, tRect), mVelocity{velocity}, mJumpVelocity{jumpVelocity},
mTileSize{tileSize}
{
    mSprite.setPosition(posOnSpriteSetX, posOnSpriteSetY);
}
Player::~Player()
{
}

void Player::setTexture(sf::Texture &image)
{
    mSprite.setTexture(image);
}

void Player::setTexture(sf::Texture &image, int posOnSpriteSetX,
                    int posOnSpriteSetY)
{
    mSprite.setTexture(image);
    mSprite.setTextureRect({posOnSpriteSetX, posOnSpriteSetY,
            mBoundingRect.width, mBoundingRect.height});
}

void Player::handleUserInput()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        mVel.x = -mVelocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        mVel.x = mVelocity;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //std::cout << "jump" << std::endl;
        if (mIsOnGround)
        {
            //Player can jump only from ground
            mVel.y = mJumpVelocity;
            mIsOnGround = false;
        }
    }
}

void Player::update(float time, std::vector<std::string> &map)
{
    //mBoundingRect.left += mVel.x * time;
    mSprite.move(mVel.x * time, 0);
    handleCollisionWithMapObjects(map);

    if (!mIsOnGround)
    {
        mVel.y += GRAVITY * time;
    }

    //mBoundingRect.top += mVel.y * time;
    mSprite.move(0, mVel.y * time);
    
    mIsOnGround = false;
    handleCollisionWithMapObjects(false, map);

    //Animation

    mCurrentFrame += ANIMATION_SPEED * time;
    if (mCurrentFrame > 6) mCurrentFrame = 0;
    if(mVel.x == 0)
    {
        mCurrentFrame = 0;
        if(mIsLastDirRight)
            mSprite.setTextureRect({40 * int(mCurrentFrame), 244, 40, 50});
        else
            mSprite.setTextureRect({40 * int(mCurrentFrame) + 40, 244, -40, 50});
    } 
    else if (mVel.x > 0)
    {
        mSprite.setTextureRect({40 * int(mCurrentFrame), 244, 40, 50});
        mIsLastDirRight = true;
    }
    else
    {
        mSprite.setTextureRect({40 * int(mCurrentFrame) + 40, 244, -40, 50});
        mIsLastDirRight = false;
    }
    //If player touches ground

    mVel.x = 0;
}

void Player::draw(sf::RenderWindow &window, float offsetX, float offsetY)
{    
    mSprite.move(-offsetX, -offsetY);
    window.draw(mSprite);
}

void Player::handleCollisionWithMapObjects(bool isAxisX,
                                           std::vector<std::string> &map)
{
    for (int y = top() / mTileSize; y < bottom() / mTileSize; ++y)
    {
        for (int x = left() / mTileSize; x < right() / mTileSize; ++x)
        {
            std::cout << "x = " << x << " y = " << y << std::endl;
            std::cout << "map[y][x] == 'X' - > " << (map[y][x] == 'X') <<
                    std::endl;
            if (map[y][x] == 'X')
            {
                if (isAxisX)
                {
                    if (mVel.x > 0)
                        mSprite.move(x * mTileSize - w(), 0);
                        //mBoundingRect.left = x * mTileSize - mBoundingRect.width;
                    if (mVel.x < 0)
                        mSprite.move(x * mTileSize + mTileSize, 0);
                        //mBoundingRect.left = x * mTileSize + mTileSize;
                }
                else
                {
                    if (mVel.y > 0)
                    {
                        mSprite.move(0, y * mTileSize - h());
                        //mBoundingRect.top = y * mTileSize - h();
                        mVel.y = 0;
                        mIsOnGround = true;
                    }
                    if (mVel.y < 0)
                    {
                        mSprite.move(0, y * mTileSize + mTileSize);
                        //mBoundingRect.top = y * mTileSize + mTileSize;
                        mVel.y = 0;
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