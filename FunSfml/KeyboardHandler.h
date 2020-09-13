#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <functional>

class CKeyboardHandler final
{
private:
  std::map <sf::Keyboard::Key, std::vector<std::function<void()>> > m_keyboardMap;

public:
  CKeyboardHandler() = default;

  void registerKeyBoardAction(sf::Keyboard::Key key, std::function<void()> func) {
    auto vec = m_keyboardMap[key];
    vec.push_back(func);
    m_keyboardMap[key] = vec;
  }

  void execute() {
    for (auto& elem : m_keyboardMap) {
      if (!sf::Keyboard::isKeyPressed(elem.first)) {
        continue;
      }
      for (auto func : elem.second) {
        func();
      }      
    }
  }
  
};

