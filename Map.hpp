/* 
 * File:   Map.hpp
 * Author: igor
 *
 * Created on 1-10-2017, 8:00
 */

#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <string>

class Map {
public:
    explicit Map();
    Map(const Map&) = delete;
    Map(Map&&) = delete;
    Map& operator=(const Map&) = delete;
    Map& operator=(Map&&) = delete;
    virtual ~Map();
    bool loadFromFile(const std::string &fileName);
    inline const std::vector<std::string>& getData() const { return mData; }
    inline std::vector<std::string>& getData() { return mData; }
    inline auto getPlayerX() const noexcept;
    inline auto getPlayerY() const noexcept { return mPlayerY; }
    inline auto getTileSize() const noexcept { return mTileSize; }
private:
    int mPlayerX, mPlayerY;
    int mTileSize = 32;
    std::vector<std::string> mData;
};

#endif /* MAP_HPP */

