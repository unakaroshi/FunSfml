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
	float angle = M_PI_4/2;

	RandomNumberGenerator randomNumber;
	
	
	Particle particle;
	bool attached = true;
	bool initialized = false;


	int maxlevel = 7;
	

public:
	Branch(sf::Vector2f from, sf::Vector2f to, int level = 0) {
		lvl = level;
		vertex.position = from;
		vertex.color = sf::Color(255, 255, 255);
		vertices.append(vertex);
		vertex.position = to;
		vertex.color = sf::Color(255, 255, 200);
		vertices.append(vertex);

		sf::Vector2f pos{ to.x - particle.getRadius(), to.y - particle.getRadius() };
		particle.setPosition(pos);

		if ( level < maxlevel) {
			addBranches();
		}
	}

	void dropLeaves() {		
		if (!attached) {
			return;
		}

		attached = false;
		

		sf::Vector2f force{ randomNumber.getNumber(-0.1f,0.1f), randomNumber.getNumber(0.4f, 1.0f)};
		particle.applyForce(force);
		for (auto b : branches) {
			b->dropLeaves();
		}
	}


	void applyWind(sf::Vector2f *wind = nullptr) {
		if (wind) {
			sf::Vector2f w(*wind);
			sf::Vector2f w2(randomNumber.getNumber(-0.005f, 0.005f), 0.0);
			w = w + w2;
			particle.applyForce(w);
			for (auto b : branches) {
				b->applyWind(wind);
			}
		}
		else {
		
			sf::Vector2f w(randomNumber.getNumber(-0.0101f, 0.01f), 0.0);
			particle.applyForce(w);
			for (auto b : branches) {
				b->applyWind(&w);
			}

		}


		
	}

	void incAngle() {
		if (lvl > 0) {
			angle += 0.01;
		}
		for (auto b : branches) {
			b->incAngle();
		}
	}

	void decAngle() {
		if (lvl > 0) {
			angle -= 0.01;			
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

		//b->update();
	}

	void update() {
		if (attached || !initialized ) {
			auto from = vertices[0].position;
			auto to = vertices[1].position;
			sf::Vector2f pos{ to.x - particle.getRadius(), to.y - particle.getRadius() };
			particle.setPosition(pos);
			initialized = true;
		}
		if (!attached) {
			applyWind();
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
		

	

	}


	void draw(sf::RenderWindow& window) {				
		window.draw(vertices);
		//auto from = vertices[0].position;
		//auto to = vertices[1].position;

		//circle1.setPosition(to.x - circle1.getRadius(), to.y - circle1.getRadius());
		
		//window.draw(circle1);
		particle.draw(window);
		for (auto b : branches) {
			b->draw(window);					
		}
		

	}
	
};

