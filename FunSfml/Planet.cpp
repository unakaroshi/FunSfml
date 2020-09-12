#include "Planet.h"

void Planet::draw(sf::RenderWindow& window, sf::Transform& transform)
{
  sf::Transform tempTransform = transform;
  transform.translate(m_position.x + m_distance, m_position.y);

  window.draw(m_shape, transform);

  for (auto& planet : m_planets) {
    transform.rotate(-(planet.m_angle));
    planet.draw(window, transform);
    transform.rotate(+(planet.m_angle));
  }
  transform = tempTransform;
}
