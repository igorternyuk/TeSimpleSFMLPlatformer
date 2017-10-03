/* 
 * File:   CPlayerControl.hpp
 * Author: igor
 *
 * Created on 3 октября 2017 г., 9:13
 */

#ifndef CPLAYERCONTROL_HPP
#define CPLAYERCONTROL_HPP

#include <SFML/Graphics.hpp>
#include "../composition.hpp"
#include "CPhysics.hpp"

namespace components
{
    /* The user-controlled player needs a special component to handle
       keyboard input.*/
    struct CPlayerControl: ecs::Component
    {
        CPhysics* cPhysics{nullptr};
        float moveSpeed, jumpSpeed;
        CPlayerControl(float moveSpeed, float jumpSpeed);
        CPlayerControl() = default;
        CPlayerControl(const CPlayerControl&) = default;
        virtual ~CPlayerControl();
        virtual void init() override;
        virtual void update(float frameTime) override;
    };
}
#endif /* CPLAYERCONTROL_HPP */

