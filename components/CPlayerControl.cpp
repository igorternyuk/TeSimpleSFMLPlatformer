/* 
 * File:   CPlayerControl.cpp
 * Author: igor
 * 
 * Created on 3 октября 2017 г., 9:13
 */

#include "CPlayerControl.hpp"

components::CPlayerControl::CPlayerControl(float moveSpeed, float jumpSpeed):
moveSpeed{moveSpeed}, jumpSpeed{jumpSpeed}
{}

components::CPlayerControl::~CPlayerControl()
{}

void components::CPlayerControl::init()
{
    CPhysics = mEntity->getComponent<CPhysics>();
}

void components::CPlayerControl::update(float frameTime)
{
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
            !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        CPhysics.velocity.x = 0;
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            CPhysics.velocity.x = -moveSpeed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            CPhysics.velocity.x = moveSpeed;
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        //std::cout << "jump" << std::endl;
        if (CPhysics.cPosition->isOnGround())
        {
            //Player can jump only from ground
            CPhysics.velocity.y = jumpSpeed;
            CPhysics.cPosition->isOnGround = false;
        }
    }
}