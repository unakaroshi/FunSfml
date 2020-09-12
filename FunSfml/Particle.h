#pragma once
#include <SFML/Graphics.hpp>

class Particle 
{
private:  
  
  sf::Vector2f m_velocity{ 0,0 };
  sf::Vector2f m_acceleration{ 0,0 };
  sf::CircleShape m_shape;

public:
  Particle(const sf::Vector2f& pos = { 0,0 }, float r = 6) {
    m_shape.setPosition(pos);
    m_shape.setRadius(r);
  }

  void setPosition(const sf::Vector2f& pos) {    
    m_shape.setPosition(pos);
  }

  void setRadius(float r) {
    m_shape.setRadius(r);
  }

  float getRadius() const {
    return m_shape.getRadius();
  }

  void applyForce(const sf::Vector2f &v) { 
    m_acceleration = m_acceleration + v;
  }

  void update() {
    m_velocity = m_velocity + m_acceleration;    
    m_acceleration = m_acceleration * 0.0f;
    auto pos = m_shape.getPosition();
    pos = pos + m_velocity;
    if (pos.y > 900) {
      pos.y = 900;
    }
    setPosition(pos);

  }

  void draw(sf::RenderWindow& window) {
    window.draw(m_shape);
  }

};