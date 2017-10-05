/* 
 * File:   Level.hpp
 * Author: igor
 *
 * Created on 1-10-2017, 8:00
 */

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <memory>

class Level {
public:
    struct EntityData
    {
        sf::IntRect bRectOnSpriteSet;
        int numFrames;
        int frameStep;
        float moveSpeed, jumpVelocity;
    };
    explicit Level();
    Level(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(const Level&) = delete;
    Level& operator=(Level&&) = delete;
    virtual ~Level();
    bool loadFromFile(const std::string &fileName);
    inline const std::string& getPathToSpriteSet() const noexcept
    {
        return mPathToImage;
    }
    inline auto getTileSize() const noexcept { return mTileSize; }
    inline auto getGravity() const noexcept { return mGravity; }
    inline const sf::Vector2f& getPlayerPos() const noexcept
    {
        return mMarioPosition;
    }
    
    inline const std::vector<sf::Vector2f>& getEnemyPositions() const noexcept
    { 
        return mEnemyPositions; 
    }
    
    inline const EntityData& getPlayerData() const noexcept
    { 
        return mPlayerData; 
    }
    
    inline const EntityData& getEnemyData() const noexcept
    { 
        return mEnemyData;
    }   

    inline const auto& getTiles() const noexcept { return mTiles; }
    inline auto getNumberOfPoints() const noexcept { return mNumPoins; }
    inline const std::vector<std::string>& getMap() const { return mMap; }
    inline std::vector<std::string>& getMap() { return mMap; }
   
private:
    
    std::string mPathToImage;
    int mTileSize;
    float mGravity;
    EntityData mPlayerData, mEnemyData;    
    sf::Vector2f mMarioPosition;
    std::vector<sf::Vector2f> mEnemyPositions;
    std::map<char, sf::IntRect> mTiles;
    int mNumPoins;
    std::vector<std::string> mMap;
};

#endif /* Level_HPP */

