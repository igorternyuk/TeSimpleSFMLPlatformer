/* 
 * File:   CPlayerControl.cpp
 * Author: igor
 * 
 * Created on 3 октября 2017 г., 9:13
 */

#include "CPlayerControl.hpp"

components::CPlayerControl::CPlayerControl(float moveSpeed, float jumpVelocity):
moveSpeed{moveSpeed}, jumpVelocity{jumpVelocity}
{}

components::CPlayerControl::~CPlayerControl()
{}

void components::CPlayerControl::init()
{
    cPhysics = &mEntity->getComponent<CPhysics>();
}

void components::CPlayerControl::update(float frameTime)
{
    controlFunc();
}