/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test.hpp
 * Author: igor
 *
 * Created on 2 октября 2017 г., 11:02
 */

#ifndef TEST_HPP
#define TEST_HPP
#include <bitset>

namespace cmp
{
    using ComponentID = std::size_t;
using GroupID = std::size_t;
constexpr std::size_t maxComponents{32};
using ComponentBitset = std::bitset<maxComponents>;

    namespace Internal
    {
        inline ComponentID getUniqueComponentID()
        {
            static ComponentID lastID{0u};
            return lastID++;
        }
    }

    template <typename T>
    inline ComponentID getComponentTypeID()
    {
        static_assert(std::is_base_of<Component, T>::value,
                "T must inherit from Component");
        static ComponentID typeID{Internal::getUniqueComponentID()};
        return typeID;
    }
}

#endif /* TEST_HPP */

