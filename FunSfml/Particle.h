#pragma once
#include <SFML/Graphics.hpp>
#include <algorithm>

class Particle 
{
private:  
  
  sf::Vector2f m_velocity{ 0,0 };
  sf::Vector2f m_acceleration{ 0,0 };
  sf::CircleShape m_shape;
  float m_mass{ 1.0 };
  float m_bottom{ 1000.0 };
  float m_bottomFriction{ 0.8 };

public:
  Particle(const sf::Vector2f& pos = { 0,0 }, float r = 6) {
    m_shape.setPosition(pos);
    m_shape.setRadius(r);
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

  void update() {
    m_velocity = m_velocity + m_acceleration;    

    if (isAtBottom()) {
      m_velocity.x = m_velocity.x * m_bottomFriction;
    }
    m_acceleration = m_acceleration * 0.0f;


    auto pos = m_shape.getPosition();
    pos = pos + m_velocity;    
    setPosition(pos);
  }

  void draw(sf::RenderWindow& window) {
    window.draw(m_shape);
  }

};