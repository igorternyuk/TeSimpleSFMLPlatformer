/* 
 * File:   Player.hpp
 * Author: igor
 *
 * Created on 30-09-2017, 11:27
 */

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    //Reminder: boundingRect an velocity should be passed via contructor
    explicit Player(int tileSize = 32);
    explicit Player(sf::Texture &image, int tileSize = 32);
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(Player&&) = delete;
    virtual ~Player();
    inline float getX() const noexcept { return mBoundingRect.left; }
    inline float getY() const noexcept { return mBoundingRect.top; }
    void setTexture(sf::Texture &image);
    void handleUserInput();
    void update(float time, const std::vector<std::string> &map);
    void draw(sf::RenderWindow &window) const;
private:
    enum class Direction{X, Y};
    void handleCollision(Direction dir, const std::vector<std::string> &map);
    float mVelX, mVelY;
    float mCurrentFrame;
    sf::FloatRect mBoundingRect;
    int mTileSize;
    sf::Sprite mSprite;
    bool mIsOnGround = false;
    bool isLastDirRight = true;
    const float VELOCITY = 90;
    const float JUMP_VELOCITY = -230.0;
    const float GRAVITY = 200;
    const float ANIMATION_SPEED = 6;

};

#endif /* PLAYER_HPP */

