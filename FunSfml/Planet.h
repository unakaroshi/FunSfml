#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
class Planet
{
private:
  float m_speed = 100.0;
  float m_rot = 0;
  float m_angle{ 0.0 };
  int m_distance{ 0 };


  sf::Vector2f m_position;
  sf::CircleShape m_shape;

  std::vector<Planet> m_planets;


public:
  Planet(int size, float angle, int distance, int x = 0, int y = 0)
    : m_angle(angle), m_distance(distance) {

    m_position.x = x;
    m_position.y = y;
    // m_shape.setPosition(m_position);
    m_shape.setOrigin(size, size);
    m_shape.setRadius(size);
    m_shape.setPointCount(500);

  }

  void addPlanet(const Planet& p) {
    Planet planet = p;
    planet.m_distance += m_shape.getRadius();
    m_planets.emplace_back(planet);
  }


  void update(sf::Time dt) {
    m_angle += m_speed * dt.asSeconds();
    m_angle = std::fmod(m_angle, 360.0);
    for (auto& planet : m_planets) {
      planet.update(dt);
    }
  }

  void draw(sf::RenderWindow& window, sf::Transform& transform);

};

