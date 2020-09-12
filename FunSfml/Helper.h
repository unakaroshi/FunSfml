#pragma once

#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <math.h>

float magnitude(const sf::Vector2f &v) {
  return std::sqrtf(v.x * v.x + v.y * v.y);
}

sf::Vector2f unitVector(const sf::Vector2f& v) {
  return v / magnitude(v);
}

sf::Vector2f rotate(const sf::Vector2f& v, float angle) {
  auto newx = std::cosf(angle) * v.x - std::sinf(angle) * v.y;
  auto newy = std::cosf(angle) * v.y + std::sinf(angle) * v.x;
  return { newx, newy };
}

