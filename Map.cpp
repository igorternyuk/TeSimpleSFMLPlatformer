/* 
 * File:   Map.cpp
 * Author: igor
 * 
 * Created on 1-10-2017, 8:00
 */

#include "Map.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Map::Map() {
}

Map::~Map() {
}

bool Map::loadFromFile(const std::string &fileName)
{
    /*[Player intial position]
5 13
[Tile size]
16
[Map]*/
    enum class LoadState{PLAYER_POSITION, TILE_SIZE, MAP};
    mData.clear();
    LoadState ls = LoadState::PLAYER_POSITION;
    std::ifstream fi(fileName);
    std::stringstream ss;
    if(fi.is_open())
    {
        while(!fi.eof())
        {
            std::string line;
            std::getline(fi, line);
            if(!line.length()) continue;
            if(line == "[Player intial position]")
            {
                ls = LoadState::PLAYER_POSITION;
                continue;
            }
            else if(ls == "[Tile size]")
            {
                ls = LoadState::TILE_SIZE;
                continue;
            }
            else if(ls == "[Map]")
            {
                ls = LoadState::MAP;
                continue;
            }
            switch(ls)
            {
            case LoadState::PLAYER_POSITION:
                ss << line;
                ss << mPlayerX >> mPlayerY;
                break;
            case LoadState::TILE_SIZE:
                mTileSize = std::atoi(line.c_str());
                break;
            case LoadState::MAP:
                mData.push_back(line);
                break;
            default:
                break;
            }
        }
        fi.close();
        return true;
    }
    else
    {
        std::cerr << "Could not load map from file: " << fileName << std::endl;
        return false;
    }
}

