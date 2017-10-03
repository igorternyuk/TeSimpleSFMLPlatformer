/* 
 * File:   CAnimation.cpp
 * Author: igor
 * 
 * Created on 3 октября 2017 г., 9:12
 */

#include "CAnimation.hpp"

components::CAnimation::CAnimation(Game *game, sf::Texture& img,
        float posOnSpriteSheetX, float posOnSpriteSheetY,float animSpeed,
        int numFrames):
game{game}, posOnSpriteSheet{posOnSpriteSheetX, posOnSpriteSheetY},
animSpeed{animSpeed}, numFrames{numFrames}
{
    sprite.setTexture(img);
}

components::CAnimation::~CAnimation() {
}

void components::CAnimation::init()
{
    cPhysics = mEntity->getComponent<CPhysics>();    
}

void components::CAnimation::update(float frameTime)
{
    sprite.setPosition(cPhysics->left(), cPhysics->top());
    if(isPlaying)
    {
        currentFrameTime += frameTime;
        if(currentFrameTime >= animSpeed)
        {
            currentFrameTime = 0.f;
            ++currentFrame;
            if(currentFrame > numFrames) currentFrame = 0;
            
            sf::IntRect destFaceRight
            {
                posOnSpriteSheet.x + cPhysics->w() * currentFrame,
                posOnSpriteSheet.y,
                cPhysics->w(),
                cPhysics->h()
            };

            sf::IntRect destFaceLeft
            {
                posOnSpriteSheet.x + cPhysics->w() * currentFrame + cPhysics->w(),
                posOnSpriteSheet.y,
                -cPhysics->w(),
                cPhysics->h()
            };
                
            if(cPhysics->velocity.x == 0)
            {
                currentFrame = 0;
                currentFrameTime = 0.f;
                isPlaying = false;
                if(isLastDirRight)
                    sprite.setTextureRect(destFaceRight);
                else
                    sprite.setTextureRect(destFaceLeft);
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
    }
}

virtual void components::CAnimation::draw()
{
    sprite.move(game->getCameraX(),  game->getCameraY());
    game->render(sprite);
}
