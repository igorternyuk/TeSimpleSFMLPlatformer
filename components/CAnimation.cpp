/* 
 * File:   CAnimation.cpp
 * Author: igor
 * 
 * Created on 3 октября 2017 г., 9:12
 */

#include "CAnimation.hpp"
#include "../Game.hpp"
#include <iostream>

components::CAnimation::CAnimation(Game *game, sf::Texture& img,
        int posOnSpriteSheetX, int posOnSpriteSheetY,float animSpeed,
        int numFrames, int frameStep):
game{game}, posOnSpriteSheet{posOnSpriteSheetX, posOnSpriteSheetY},
animSpeed{animSpeed}, numFrames{numFrames}, frameStepOnSpriteSetPX{frameStep}
{
    sprite.setTexture(img);
}

components::CAnimation::~CAnimation() {
}

void components::CAnimation::init()
{
    cPhysics = &mEntity->getComponent<CPhysics>();    
}

void components::CAnimation::update(float frameTime)
{
    if(isPhysicsUninitialized)
    {
        sprite.setTextureRect(sf::IntRect(posOnSpriteSheet.x,
                                          posOnSpriteSheet.y, cPhysics->w(),
                                          cPhysics->h()));
        isPhysicsUninitialized = false;
    }
    sprite.setPosition(cPhysics->left(), cPhysics->top());
    //if(isPlaying)
    //{
        currentFrameTime += frameTime;
        if(currentFrameTime >= animSpeed)
        {
            currentFrameTime = 0.f;
            ++currentFrame;
            if(currentFrame >= numFrames - 1) currentFrame = 0;
            
            sf::IntRect destFaceRight
            {
                posOnSpriteSheet.x + frameStepOnSpriteSetPX * currentFrame,
                posOnSpriteSheet.y,
                cPhysics->w(),
                cPhysics->h()
            };
            
            sf::IntRect destFaceLeft
            {
                posOnSpriteSheet.x + frameStepOnSpriteSetPX * currentFrame +
                cPhysics->w(),
                posOnSpriteSheet.y,
                -cPhysics->w(),
                cPhysics->h()
            };
                
            if(cPhysics->velocity.x == 0)
            {
                currentFrame = 0;
                currentFrameTime = 0.f;
                isPlaying = false;
                sf::IntRect rect
                {
                    posOnSpriteSheet.x - frameStepOnSpriteSetPX +
                    (isLastDirRight ? 0 : 1) * cPhysics->w(),
                    posOnSpriteSheet.y,
                    cPhysics->w() * (isLastDirRight ? +1 : -1),
                    cPhysics->h()                            
                };
                sprite.setTextureRect(rect);
            } 
            else if (cPhysics->velocity.x > 0)
            {
                sprite.setTextureRect(destFaceRight);
                isLastDirRight = true;
            }
            else
            {
                sprite.setTextureRect(destFaceLeft);
                isLastDirRight = false;
            }
        }
    //}
}

void components::CAnimation::draw()
{
    sprite.setPosition(cPhysics->cPosition->pos.x - game->getCameraX(),
                       cPhysics->cPosition->pos.y - game->getCameraY());
    if(!isPhysicsUninitialized)
        game->render(sprite);
}
