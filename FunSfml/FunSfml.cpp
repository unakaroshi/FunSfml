// Pong.cpp : Defines the entry point for the console application.
//

#define _USE_MATH_DEFINES

#include "Planet.h"
#include "Branch.h"
#include <sstream>
#include <cstdlib>
#include <random>
#include <math.h>
#include <iostream>
#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>




int main() {

	sf::Vector2f velocity_sun(0.0, 0.0);

	float scale = 1.0f;
	std::default_random_engine defEngine;
	std::uniform_real_distribution<float> floatDistro(0.0f, 360.0f);
	std::uniform_real_distribution<float> floatDistro2(2.0f, 4.0f);

	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	for (std::size_t i = 0; i < modes.size(); ++i)
	{
		sf::VideoMode mode = modes[i];
		std::cout << "Mode #" << i << ": "
			<< mode.width << "x" << mode.height << " - "
			<< mode.bitsPerPixel << " bpp" << std::endl;
	}
	// Create a window with the same pixel depth as the desktop
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	
	sf::RenderWindow window(modes[0], "SFML window" /*, sf::Style::Fullscreen*/);
	
	ImGui::SFML::Init(window);
	
	sf::Color bgColor;
	float color[3] = { 0.f, 0.f, 0.f };


	Planet sun(40, 0, 0, modes[0].width / 2, modes[0].height / 2);
	Planet mercury(5, floatDistro(defEngine), 20);
	Planet venus(8, floatDistro(defEngine), 50);
	Planet earth(8, floatDistro(defEngine), 110);
	Planet moon(3, floatDistro(defEngine), 10);

	earth.addPlanet(moon);
	sun.addPlanet(mercury);
	sun.addPlanet(venus);
	sun.addPlanet(earth);

	sf::Vector2f s(floatDistro2(defEngine), floatDistro2(defEngine));



	// A cool retro-style font
	//sf::Font font;
	//font.loadFromFile("fonts/DS-DIGI.ttf");

	//sf::Text text;
	//text.setFont(font);
	//text.setPosition(1, 1);
	//text.setString("Hallo");



	// Here is our clock for timing everything
	sf::Clock clock;
	//sf::Clock deltaClock;
	window.setFramerateLimit(120);

	sf::Vector2f from{ 900,1040 };
	sf::Vector2f to{ 900,700 };

	Branch branch(from, to);

	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);
			// Quit the game when the window is closed
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}




		// Handle the player quitting
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();
		}

		// Handle the pressing and releasing of the arrow keys
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			branch.incAngle();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			branch.decAngle();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
			branch.dropLeaves();
		}
		// Update the delta time
		sf::Time deltaTime = clock.restart();

		ImGui::SFML::Update(window, deltaTime);
		ImGui::Begin("Sample window"); // begin window

																			 // Background color edit
		if (ImGui::ColorEdit3("Background color", color)) {
			// this code gets called if color value changes, so
			// the background color is upgraded automatically!
			bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
			bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
			bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		}
		ImGui::End(); // end window



	
	
		sun.update(deltaTime);

		velocity_sun.x += s.x * deltaTime.asSeconds() * 300.0;
		velocity_sun.y += s.y * deltaTime.asSeconds() * 300.0;

		sf::Vector2f pos(std::sinf(M_2_PI * (velocity_sun.x / 360.0)) * 900, std::sinf(M_2_PI * (velocity_sun.y / 360.0)) * 500);


		sf::Transform planetTransform = sf::Transform::Identity;
		planetTransform.translate(pos);
		planetTransform.scale(scale, scale, modes[0].width / 2, modes[0].height / 2);
		
		window.clear(bgColor);

		sun.draw(window, planetTransform);

		branch.update();
		
		branch.draw(window);
		//window.draw(text);

		ImGui::SFML::Render(window);
		
		window.display();
	}

	return 0;
}
