/* 
 * File:   MapManager.hpp
 * Author: igor
 *
 * Created on 30-09-2017 г., 14:16
 */

#ifndef MAPMANAGER_HPP
#define MAPMANAGER_HPP

#include <vector>
#include <string>

class MapManager {
public:
    MapManager();
    MapManager(const MapManager& orig) = delete;
    virtual ~MapManager();
private:
    std::vector<std::string> mMap;
};

#endif /* MAPMANAGER_HPP */

