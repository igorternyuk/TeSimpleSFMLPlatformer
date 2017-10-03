/* 
 * File:   CPosition.cpp
 * Author: igor
 * 
 * Created on 2-10-2017, 13:36
 */

#include "CPosition.hpp"

components::CPosition::CPosition() {
}

components::CPosition::CPosition(const sf::Vector2f &pos, Map& map):
pos{pos},map{map}
{}

components::CPosition::~CPosition() {
}

