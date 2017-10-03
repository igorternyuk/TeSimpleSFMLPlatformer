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
    explicit Player(sf::Texture &image, const sf::IntRect &tRect,
                    float velocity, float jumpVelocity, int posOnSpriteSetX = 0,
                    int posOnSpriteSetY = 0, int tileSize = 32);
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) = delete;
    Player& operator=(Player&&) = delete;
    virtual ~Player();
    inline auto x() const noexcept { return mSprite.getGlobalBounds().left; }
    inline auto y() const noexcept { return mSprite.getGlobalBounds().top; }
    inline auto top() const noexcept { return mSprite.getGlobalBounds().top; }
    inline auto left() const noexcept { return mSprite.getGlobalBounds().left;}
    inline auto bottom() const noexcept { return mSprite.getGlobalBounds().top +
            mSprite.getGlobalBounds().height; }
    inline auto right() const noexcept { return mSprite.getGlobalBounds().left +
            mSprite.getGlobalBounds().width; }
    inline auto w() const noexcept { return mSprite.getGlobalBounds().width; }
    inline auto h() const noexcept { return mSprite.getGlobalBounds().height; }
    const sf::FloatRect& boundingRect() const noexcept 
    { return mSprite.getGlobalBounds().; }
    inline void die() { mIsAlive = false; }
    void setTexture(sf::Texture &image);
    void setTexture(sf::Texture &image, int posOnSpriteSetX,
                    int posOnSpriteSetY);
    void handleUserInput();
    void update(float time, std::vector<std::string> &map);
    void draw(sf::RenderWindow &window, float offsetX, float offsetY);

private:
    void handleCollisionWithMapObjects(bool isAxisX = true,
                                       std::vector<std::string> &map);
    sf::Sprite mSprite;
    float mVelocity;
    float mJumpVelocity;
    int mTileSize;
    sf::Vector2f mVel{0.f,0.f};
    float mCurrentFrame{0.f};
    bool mIsOnGround {false};
    bool mIsLastDirRight {true};
    bool mIsAlive {true};
    bool mIsAxisXCollsiionsCheck {true};
    const float GRAVITY {200.f};
    const float ANIMATION_SPEED {6.f};
};

#endif /* PLAYER_HPP */

