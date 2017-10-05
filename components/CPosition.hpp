/* 
 * File:   CPosition.hpp
 * Author: igor
 *
 * Created on 2-10-2017., 13:36
 */

#ifndef CPOSITION_HPP
#define CPOSITION_HPP

#include <SFML/Graphics.hpp>
#include "../composition.hpp"

namespace components
{
    struct CPosition: ecs::Component {
        sf::Vector2f pos;
        bool isOnGround{true};
        CPosition() = default;
        CPosition(const CPosition&) = default;
        CPosition(CPosition&&) = default;
        CPosition& operator=(const CPosition&) = default;
        CPosition& operator=(CPosition&&) = default;
        CPosition(const sf::Vector2f &pos);
        virtual ~CPosition();
        inline auto x() const noexcept { return pos.x; }
        inline auto y() const noexcept { return pos.y; }
        //inline auto onGround() const noexcept { return isOnGround; }
    };
}

#endif /* CPOSITION_HPP */

