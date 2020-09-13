#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>

class CKeyboardHandler final
{
private:
  std::vector<std::pair<sf::Keyboard::Key, std::function<void()>>> m_keyboardMap;

public:
  CKeyboardHandler() = default;
  ~CKeyboardHandler() = default;

  void registerKeyBoardAction(sf::Keyboard::Key key, std::function<void()> func) {
    m_keyboardMap.emplace_back(std::make_pair(key, func));
  }

  void execute() {
    for (auto& elem : m_keyboardMap) {
      if (sf::Keyboard::isKeyPressed(elem.first)) {
        elem.second();
      }
    }
  }
  
};

