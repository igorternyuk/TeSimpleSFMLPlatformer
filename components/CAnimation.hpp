#ifndef CANIMATION_HPP
#define CANIMATION_HPP

#include <SFML/Graphics.hpp>
#include "../composition.hpp"
#include "CPhysics.hpp"

namespace components
{
    struct CAnimation: ecs::Component 
    {
        Game* game{nullptr};
        CPhysics *cPhysics{nullptr};
        sf::Sprite sprite;
        sf::Vector2i posOnSpriteSheet;
        int currentFrame{0}, numFrames;
        float currentFrameTime{0.f}, animSpeed;
        bool isLastDirRight{true};
        bool isPlaying{true};
        CAnimation(Game *game, sf::Texture& img, int posOnSpriteSheetX,
        int posOnSpriteSheetY,float animSpeed, int numFrames);
        CAnimation() = default;
        CAnimation(const CAnimation&) = default;
        CAnimation& operator=(const CAnimation&) = default;
        virtual ~CAnimation();
        virtual void init() override;
        virtual void update(float frameTime) override;
        virtual void draw() override;
    };
}

#endif /* CANIMATION_HPP */

