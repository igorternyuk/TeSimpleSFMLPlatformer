/* 
 * File:   Level.cpp
 * Author: igor
 * 
 * Created on 1-10-2017, 8:00
 */

#include "Level.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Level::Level() {
    mEnemyPositions.reserve(6);
}

Level::~Level() {
}

bool Level::loadFromFile(const std::string &fileName)
{
    enum class LoadState
    {
        PATH_TO_IMAGE,
        TILE_SIZE,
        GRAVITY,
        PLAYER_BOUNDING_RECT,
        PLAYER_NUM_OF_FRAMES,
        PLAYER_FRAME_STEP,
        PLAYER_MOVE_SPEED,
        PLAYER_JUMP_VELOCITY,
        PLAYER_POSITION,
        ENEMY_BOUNDING_RECT,
        ENEMY_NUM_OF_FRAMES,
        ENEMY_FRAME_STEP,
        ENEMY_MOVE_SPEED,
        ENEMY_JUMP_VELOCITY,
        ENEMY_MAP_POSITIONS,
        TILES,
        MAP       
    };

    mEnemyPositions.clear();
    mTiles.clear();
    mMap.clear();
    
    LoadState ls = LoadState::PLAYER_POSITION;
    std::ifstream fi(fileName);
    std::stringstream ss;
    
    if(fi.is_open())
    {
        while(!fi.eof())
        {
            std::string line;
            std::getline(fi, line);
            if(!line.length() || line[0] == '#') continue;
            
            if(line == "[Path to image]")
            {
                ls = LoadState::PATH_TO_IMAGE;
                continue;
            }
            else if(line == "[Tile size]")
            {
                ls = LoadState::TILE_SIZE;
                continue;
            }
            else if(line == "[Gravity]")
            {
                ls = LoadState::GRAVITY;
                continue;
            }
            else if(line == "[Player bounding rect on the sprite]")
            {
                ls = LoadState::PLAYER_BOUNDING_RECT;
                continue;
            }
            else if(line == "[Player number of frames]")
            {
                ls = LoadState::PLAYER_NUM_OF_FRAMES;
                continue;
            }
            else if(line == "[Player frame step]")
            {
                ls = LoadState::PLAYER_FRAME_STEP;
                continue;
            }
            else if(line == "[Player move speed]")
            {
                ls = LoadState::PLAYER_MOVE_SPEED;
                continue;
            }
            else if(line == "[PLayer jump velocity]")
            {
                ls = LoadState::PLAYER_JUMP_VELOCITY;
                continue;
            }
            else if(line == "[Player map position]")
            {
                ls = LoadState::PLAYER_POSITION;
                continue;
            }
            else if(line == "[Enemy bounding rect on the sprite]")
            {
                ls = LoadState::ENEMY_BOUNDING_RECT;
                continue;
            }
            else if(line == "[Enemy number of frames]")
            {
                ls = LoadState::ENEMY_NUM_OF_FRAMES;
                continue;
            }
            else if(line == "[Enemy frame step]")
            {
                ls = LoadState::ENEMY_FRAME_STEP;
                continue;
            }
            else if(line == "[Enemy move speed]")
            {
                ls = LoadState::ENEMY_MOVE_SPEED;
                continue;
            }
            else if(line == "[Enemy jump velocity]")
            {
                ls = LoadState::ENEMY_JUMP_VELOCITY;
                continue;
            }
            else if(line == "[Enemy map positions]")
            {
                ls = LoadState::ENEMY_MAP_POSITIONS;
                continue;
            }
            else if(line == "[Tiles]")
            {
                ls = LoadState::TILES;
                continue;
            }
            else if(line == "[Map]")
            {
                ls = LoadState::MAP;
                continue;
            }          
            
            sf::Vector2f pos;
            char key;
            sf::IntRect tileRect;
            
            
            switch(ls)
            {
            case LoadState::PATH_TO_IMAGE :
                mPathToImage = line;
                break;
            case LoadState::TILE_SIZE :
                mTileSize = std::atoi(line.c_str());
                break;
            case LoadState::GRAVITY :
                ss.clear();
                ss << line;
                ss >> mGravity;
                break;
            case LoadState::PLAYER_BOUNDING_RECT :                
                ss.clear();
                ss << line;
                ss >> mPlayerData.bRectOnSpriteSet.left >>
                       mPlayerData.bRectOnSpriteSet.top >> 
                       mPlayerData.bRectOnSpriteSet.width >> 
                       mPlayerData.bRectOnSpriteSet.height;                
                break;
            case LoadState::PLAYER_NUM_OF_FRAMES :
                mPlayerData.numFrames = std::atoi(line.c_str());
                break;
            case LoadState::PLAYER_FRAME_STEP :
                mPlayerData.frameStep = std::atoi(line.c_str());
                break;
            case LoadState::PLAYER_MOVE_SPEED :
                ss.clear();
                ss << line;                
                ss >> mPlayerData.moveSpeed;
                break;
            case LoadState::PLAYER_JUMP_VELOCITY :
                ss.clear();
                ss << line;
                ss >> mPlayerData.jumpVelocity;
                break;
            case LoadState::PLAYER_POSITION :
                ss.clear();
                ss << line;
                ss >> mMarioPosition.x >> mMarioPosition.y;
                mMarioPosition.x *= mTileSize;
                mMarioPosition.y *= mTileSize;
                break;
            case LoadState::ENEMY_BOUNDING_RECT :
                ss.clear();
                ss << line;
                ss >> mEnemyData.bRectOnSpriteSet.left >>
                       mEnemyData.bRectOnSpriteSet.top >> 
                       mEnemyData.bRectOnSpriteSet.width >> 
                       mEnemyData.bRectOnSpriteSet.height; 
                break;
            case LoadState::ENEMY_NUM_OF_FRAMES :
                mEnemyData.numFrames = std::atoi(line.c_str());
                break;
            case LoadState::ENEMY_FRAME_STEP :
                mEnemyData.frameStep = std::atoi(line.c_str());
                break;
            case LoadState::ENEMY_MOVE_SPEED :
                ss.clear();
                ss << line;
                ss >> mEnemyData.moveSpeed;
                break;
            case LoadState::ENEMY_JUMP_VELOCITY :
                ss.clear();
                ss << line;
                ss >> mEnemyData.jumpVelocity;
                break;
            case LoadState::ENEMY_MAP_POSITIONS :
                ss.clear();
                ss << line;
                ss >> pos.x >> pos.y;
                pos.x *= mTileSize;
                pos.y *= mTileSize;
                mEnemyPositions.push_back(pos);
                break;
            case LoadState::TILES :
                ss.clear();
                ss << line;
                ss >> key >> tileRect.left >> tileRect.top >> tileRect.width >>
                tileRect.height;
                mTiles.insert({key, tileRect});
                break;
            case LoadState::MAP :
                mMap.push_back(line);
                break;
            }
        }
        fi.close();
        return true;
    }
    else
    {
        std::cerr << "Could not load Level from file: " << fileName << std::endl;
        return false;
    }
}

