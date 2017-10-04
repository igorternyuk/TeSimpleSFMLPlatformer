/* 
 * File:   CPhycics.cpp
 * Author: igor
 * 
 * Created on 2-10-2017, 13:57
 */

#include "CPhysics.hpp"
#include <iostream>

components::CPhysics::CPhysics(float gravity, sf::Vector2f size):
gravity{gravity}, size{size}
{}

components::CPhysics::~CPhysics()
{}

void components::CPhysics::init()
{
    cPosition = &mEntity->getComponent<CPosition>();
}

void components::CPhysics::update(float frameTime)
{
    moveFunc(frameTime);
    /*cPosition->pos.x += velocity.x * frameTime;
    //Check collision with respect to X-axis
    onCollision(true);
    if(!cPosition->isOnGround())
    {
        velocity.y += gravity * frameTime;
    }
    cPosition->pos.y += velocity.y * frameTime;
    cPosition->isOnGround = false;
    //Check collision with respect to Y-axis
    onCollision(false);*/
}
