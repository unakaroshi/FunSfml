#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <numeric>

class Particle 
{
private: 
  sf::Vector2f m_home;
  sf::Vector2f m_velocity{ 0,0 };
  sf::Vector2f m_acceleration{ 0,0 };
  sf::CircleShape m_shape;

  bool m_movingHome = false;
  bool m_attached = true;

  float m_mass{ 1.0f };
  float m_bottom{ 1000.0f };
  float m_bottomFriction{ 0.8f };

public:
  Particle(const sf::Vector2f& pos = { 0.0f,0.0f }, float r = 6.0f) {
    setHomePosition(pos);
    m_shape.setPosition(pos);
    m_shape.setRadius(r);
  }

  void setAttached(bool flag) {
    m_attached = flag;
  }

  bool isAttached() {
    return m_attached;
  }

  void setHomePosition(const sf::Vector2f& pos) {
    m_home = pos;
  }

  void moveHome() {
    m_movingHome = true;
    m_velocity *= 0.0f;
    m_acceleration *= 0.0f;
  }

  void setMovingHome(bool moving) {
    m_movingHome = moving;
  }

  bool isAtBottom() {
    return (m_shape.getPosition().y >= m_bottom);
  }

  void setBottomFriction(float friction) {
    m_bottomFriction = friction;
  }

  sf::Vector2f limitToBottom(const sf::Vector2f &v) {    
    auto y = std::min(v.y, m_bottom);    
    return { v.x, y };
  }


  void setVelocity(const sf::Vector2f& vel) {
    m_velocity = vel;  
  }

  void setPosition(const sf::Vector2f& pos) { 
    m_shape.setPosition(limitToBottom(pos));
  }

  void setMass(float m) {
    m_mass = m;
  }

  float getMass() const {
    return m_mass;
  }

  void setRadius(float r) {
    m_shape.setRadius(r);
  }

  float getRadius() const {
    return m_shape.getRadius();
  }

  void applyGravity(const sf::Vector2f &v) {
    auto v2 = v * m_mass;
    applyForce(v2);
  }
 
  void applyWind(const sf::Vector2f &v) {
    if (m_mass == 0.0f) {
      return;
    }
    auto v2 = v / m_mass;
    applyForce(v2);
  }

  void applyForce(const sf::Vector2f &v) { 
    m_acceleration = m_acceleration + v;
  }

  void updateNormal() {
    if (!m_attached) {
      m_velocity = m_velocity + m_acceleration;
      m_velocity.x *= 0.9f;
      if (isAtBottom()) {
        m_velocity.x = m_velocity.x * m_bottomFriction;
      }
    }
    
    auto pos = m_shape.getPosition();
    pos = pos + m_velocity;
    setPosition(pos);
  }

  void updateMoveHome() {
    auto pos = m_shape.getPosition();
    //pos = m_home;
    pos.x = std::lerp(pos.x, m_home.x, 0.1f);
    pos.y = std::lerp(pos.y, m_home.y, 0.1f);

    if (std::fabsf(pos.x - m_home.x) < 0.01f && std::fabsf(pos.y - m_home.y) < 0.01f) {
      m_attached = true;
      m_movingHome = false;
      pos = m_home;
    }
    m_shape.setPosition(pos); 
    
  }

  void update() {
    if (m_movingHome) {
      updateMoveHome();
    }
    else {
      updateNormal();
    }
    m_acceleration = m_acceleration * 0.0f;
  }

  void draw(sf::RenderWindow& window) {
    window.draw(m_shape);
  }

};