#pragma once
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "RandomNumberGenerator.h"

#include "Helper.h"
#include "Particle.h"

class Branch
{
private:
  sf::VertexArray vertices{ sf::Lines };
  sf::Vertex vertex;
  std::vector<Branch*> branches;
  int lvl;
  float angle{ static_cast<float>(M_PI_4) / 2.0f };

  RandomNumberGenerator randomNumber;  
  
  Particle particle;
  
  bool initialized = false;


  int maxlevel = 8;
  

public:
  Branch(sf::Vector2f from, sf::Vector2f to, int level = 0) {
    lvl = level;
    vertex.position = from;
    vertex.color = sf::Color(255, 255, 255);
    vertices.append(vertex);
    vertex.position = to;
    vertex.color = sf::Color(255, 255, 200);
    vertices.append(vertex);

    particle.setMass(randomNumber.getNumber(10.0, 50.0));	
    particle.setAttached(true);

    if ( level < maxlevel) {
      addBranches();
    }
  }
  
  void dropLeaves() {		       
    particle.setAttached(false);
    particle.setMovingHome(false);
    for (auto b : branches) {
      b->dropLeaves();
    }
  }

  void reset() {    
    auto to = vertices[1].position;
    particle.moveHome();
    for (auto b : branches) {
      b->reset();
    }
  }

  
  void applyWind(sf::Vector2f* wind = nullptr) {
    if (!wind) {
      return;
    }		
    particle.applyWind(*wind);
    for (auto b : branches) {
      b->applyWind(wind);
    }
  }

  void applyGravity(sf::Vector2f* gravity = nullptr) {
    if (!gravity) {
      return;
    }		
    particle.applyGravity(*gravity);
    for (auto b : branches) {
      b->applyGravity(gravity);
    }
  }

  void incAngle() {
    if (lvl > 0) {
      angle += 0.01f;
    }
    for (auto b : branches) {
      b->incAngle();
    }
  }

  void decAngle() {
    if (lvl > 0) {
      angle -= 0.01f;			
    }
    for (auto b : branches) {
      b->decAngle();
    }
  }



  void addBranches() {		
    ++lvl;
    auto from = vertices[0].position;
    auto to = vertices[1].position;

    sf::Vector2f toFrom(to - from);
    auto mag = magnitude(toFrom);
    auto dir = unitVector(toFrom);

    auto v = dir * mag * 0.75f;
        
    auto rotated1 = rotate(v, angle) + to;
    auto rotated2 = rotate(v, -angle) + to;

    branches.push_back(new Branch(to, rotated1, lvl));	
    branches.push_back(new Branch(to, rotated2, lvl));		
  }

  void rotateChild(Branch *b, float angle) {
    auto from = b->vertices[0].position;
    auto to = b->vertices[1].position;

    sf::Vector2f toFrom(to - from);
    auto mag = magnitude(toFrom);
    auto dir = unitVector(toFrom);

    auto v = dir * mag;

    auto rotated = rotate(v, angle) + vertices[1].position;
    b->vertices[0].position = vertices[1].position;
    b->vertices[1].position = rotated;
  }

  void update() {
    if (particle.isAttached() ||!initialized ) {
      auto from = vertices[0].position;
      auto to = vertices[1].position;
      sf::Vector2f pos{ to.x - particle.getRadius(), to.y - particle.getRadius() };
      particle.setPosition(pos);
      particle.setHomePosition(pos);
      initialized = true;
    }
    

    particle.update();
    if (lvl < 1 || branches.empty()) {			
      return;
    }

    rotateChild(branches[0], angle);
    rotateChild(branches[1], -angle);
    angle = 0;

    for (auto b : branches) {
      b->update();
    }

    auto from = vertices[0].position;
    auto to = vertices[1].position;
    sf::Vector2f p{ to.x - particle.getRadius(), to.y - particle.getRadius() };
    particle.setHomePosition(p);
    if (particle.isAttached()) {
      particle.setPosition(p);
    }
  }


  void draw(sf::RenderWindow& window) {				
    window.draw(vertices); 
    particle.draw(window);
    for (auto b : branches) {
      b->draw(window);					
    }
    

  }
  
};

