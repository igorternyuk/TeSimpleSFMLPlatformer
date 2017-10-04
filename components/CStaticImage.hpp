/* 
 * File:   CStaticImage.hpp
 * Author: igor
 *
 * Created on 3 октября 2017 г., 17:40
 */

#ifndef CSTATICIMAGE_HPP
#define CSTATICIMAGE_HPP

#include <SFML/Graphics.hpp>
#include "../composition.hpp"
#include "CPhysics.hpp"

class Game;

namespace components
{
    struct CStaticImage: ecs::Component 
    {
        Game* game{nullptr};
        CPhysics *cPhysics{nullptr};
        sf::Sprite sprite;
        sf::Vector2i posOnSpriteSheet;

        CStaticImage(Game *game, sf::Texture& img, int posOnSpriteSheetX,
        int posOnSpriteSheetY);
        //CStaticImage() = default;
        //CStaticImage(const CAnimation&) = default;
        //CStaticImage& operator=(const CStaticImage&) = default;
        virtual ~CStaticImage();
        virtual void init() override;
        virtual void update(float frameTime) override;
        virtual void draw() override;
    };
}


#endif /* CSTATICIMAGE_HPP */

