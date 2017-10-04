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
        //Наверно надо добавить указатель на game что-бы когда нейм овер 
        //марио не реагировал на нажатия кнопок
        CPhysics* cPhysics{nullptr};
        float moveSpeed, jumpVelocity;
        std::function<void()> controlFunc;
        CPlayerControl(float moveSpeed, float jumpSpeed);
        
        CPlayerControl() = default;
        CPlayerControl(const CPlayerControl&) = default;
        CPlayerControl(CPlayerControl&&) = default;
        CPlayerControl& operator=(const CPlayerControl&) = default;
        CPlayerControl& operator=(CPlayerControl&&) = default;
        virtual ~CPlayerControl();
        
        virtual void init() override;
        virtual void update(float frameTime) override;
    };
}
#endif /* CPLAYERCONTROL_HPP */

