#ifndef CANIMATION_HPP
#define CANIMATION_HPP

#include <SFML/Graphics.hpp>
#include "../composition.hpp"
#include "CPhysics.hpp"

class Game;

namespace components
{   
    struct CAnimation: ecs::Component 
    {
        Game* game{nullptr};
        CPhysics *cPhysics{nullptr};
        sf::Sprite sprite;
        sf::Vector2i posOnSpriteSheet;
        int currentFrame{0}, numFrames, frameStepOnSpriteSetPX;
        float currentFrameTime{0.f}, animSpeed;
        bool isLastDirRight{true};
        bool isPlaying{true};
        /*components::CAnimation::CAnimation(Game *game, sf::Texture& img,
        float posOnSpriteSheetX, float posOnSpriteSheetY,float animSpeed,
        int numFrames, int frameStep):*/
        CAnimation(Game *game, sf::Texture& img, int posOnSpriteSheetX,
        int posOnSpriteSheetY,float animSpeed, int numFrames, int frameStep);
       //CAnimation() = default;
       // CAnimation(const CAnimation&) = default;
        //CAnimation& operator=(const CAnimation&) = default;
        virtual ~CAnimation();
        virtual void init() override;
        virtual void update(float frameTime) override;
        virtual void draw() override;
    };
}

#endif /* CANIMATION_HPP */

