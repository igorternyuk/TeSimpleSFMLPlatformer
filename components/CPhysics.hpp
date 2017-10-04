/* 
 * File:   CPhycics.hpp
 * Author: igor
 *
 * Created on 2-10-2017, 13:57
 */

#ifndef CPHYCICS_HPP
#define CPHYCICS_HPP

#include <SFML/Graphics.hpp>
#include "../composition.hpp"
#include "CPosition.hpp"
#include <functional>

namespace components
{
    struct CPhysics: ecs::Component 
    {
        CPosition* cPosition{nullptr};
        sf::Vector2i size;
        sf::Vector2f velocity;
        float gravity;
        std::function<void(float ft)> moveFunc;
        std::function<void(bool axisX)> onCollision;
        //CPhysics() = default;
        CPhysics(float gravity, sf::Vector2f size);
        //CPhysics(const CPhysics&) = default;
        //CPhysics& operator=(const CPhysics&) = default;
        virtual ~CPhysics();
        virtual void init() override;
        virtual void update(float frameTime) override;
        inline auto left() const noexcept { return cPosition->x(); }
        inline auto top() const noexcept { return cPosition->y(); }
        inline auto right() const noexcept { return cPosition->x() + size.x; }
        inline auto bottom() const noexcept { return cPosition->y() + size.y; }
        inline auto w() const noexcept { return size.x; }
        inline auto h() const noexcept { return size.y; }
    };
}

#endif /* CPHYCICS_HPP */

