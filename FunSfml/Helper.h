#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <math.h>


/*
Gets the magnitude of a sf::vector2f
*/
float magnitude(const sf::Vector2f &v) {
  return std::sqrtf(v.x * v.x + v.y * v.y);
}

/*
Gets the unit vector of an sf::Vector2f
It the magnitude of the vector is 0, a vector (0,0) is returned.
*/
sf::Vector2f unitVector(const sf::Vector2f& v) {
  auto mag = magnitude(v);
  if (mag == 0.0) {
    return { 0.0f, 0.0f };
  }
  return v / magnitude(v);
}

/*
Rotate a sf::Vector2f with an angle (radians)
*/
sf::Vector2f rotate(const sf::Vector2f& v, float angleRad) {
  auto newx = std::cosf(angleRad) * v.x - std::sinf(angleRad) * v.y;
  auto newy = std::cosf(angleRad) * v.y + std::sinf(angleRad) * v.x;
  return { newx, newy };
}

