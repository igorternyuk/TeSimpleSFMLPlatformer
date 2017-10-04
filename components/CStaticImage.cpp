/* 
 * File:   CStaticImage.cpp
 * Author: igor
 * 
 * Created on 3 октября 2017 г., 17:40
 */

#include "CStaticImage.hpp"
#include "../Game.hpp"

components::CStaticImage::CStaticImage(Game *game, sf::Texture& img,
        int posOnSpriteSheetX, int posOnSpriteSheetY):
game{game}, posOnSpriteSheet{posOnSpriteSheetX, posOnSpriteSheetY}
{
    sprite.setTexture(img);
}

components::CStaticImage::~CStaticImage() {
}

void components::CStaticImage::init()
{
    cPhysics = &mEntity->getComponent<CPhysics>();    
}

void components::CStaticImage::update(float frameTime)
{
    sprite.setPosition(cPhysics->left(), cPhysics->top());
}

void components::CStaticImage::draw()
{
    sprite.move(game->getCameraX(),  game->getCameraY());
    game->render(sprite);
}