/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IDrawableEntity.hpp
 * Author: igor
 *
 * Created on 30-09-2017 г., 12:53
 */

#ifndef IDRAWABLEENTITY_HPP
#define IDRAWABLEENTITY_HPP

class IDrawableEntity
{
public:
    virtual ~IDrawableEntity() {}
    virtual void handleUserInput() = 0;
    virtual void update(float time) = 0;
    virtual void draw(sf::RenderWindow &window) const = 0;
};

#endif /* IDRAWABLEENTITY_HPP */

