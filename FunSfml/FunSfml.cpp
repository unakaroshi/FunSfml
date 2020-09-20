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
#include "KeyboardHandler.h"
#include "RandomNumberGenerator.h"
#include "imgui.h"
#include "imgui-SFML.h"

int main() 
{
	RandomNumberGenerator randomNumber;
	CKeyboardHandler keyboardHandler;


	sf::Vector2f velocity_sun(0.0, 0.0);


	float scale = 1.0f;
	
	

	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	for (std::size_t i = 0; i < modes.size(); ++i)
	{
		sf::VideoMode mode = modes[i];
		std::cout << "Mode #" << i << ": "
			<< mode.width << "x" << mode.height << " - "
			<< mode.bitsPerPixel << " bpp" << std::endl;
	}
	// Create a window with the same pixel depth as the desktop
	//sf::VideoMode desktop = sf::VideoMode::getDesktopMode();


	sf::RenderWindow window(modes[0], "SFML window" /*, sf::Style::Fullscreen*/);

	ImGui::SFML::Init(window);


	Branch branch(sf::Vector2f(900,1040), sf::Vector2f(900,700));

	
	keyboardHandler.registerKeyBoardAction(sf::Keyboard::Escape, [&]() {window.close(); });
	keyboardHandler.registerKeyBoardAction(sf::Keyboard::Up, [&]() { branch.incAngle(); });
	keyboardHandler.registerKeyBoardAction(sf::Keyboard::Down, [&]() { branch.decAngle(); });
	keyboardHandler.registerKeyBoardAction(sf::Keyboard::Space, [&]() { branch.dropLeaves(); });
	keyboardHandler.registerKeyBoardAction(sf::Keyboard::Enter, [&]() { branch.reset(); });
	
	
	sf::Color bgColor;
	float color[3] = { 0.0f, 0.0f, 0.0f };
	float wind { 0.01f };
	float gravity{ 0.02f };
	bool showSun{ false };


	Planet sun(40.0f, 0.0f, 0.0f, modes[0].width / 2.0f, modes[0].height / 2.0f);
	Planet mercury(5.0f, randomNumber.getNumber(0.0f, 360.0f), 20.0f);
	Planet venus(8.0f, randomNumber.getNumber(0.0f, 360.0f), 50.0f);
	Planet earth(8.0f, randomNumber.getNumber(0.0f, 360.0f), 110.0f);
	Planet moon(3.0f, randomNumber.getNumber(0.0f, 360.0f), 10.0f);

	earth.addPlanet(moon);
	sun.addPlanet(mercury);
	sun.addPlanet(venus);
	sun.addPlanet(earth);

	sf::Vector2f s(randomNumber.getNumber(2.0f, 4.0f), randomNumber.getNumber(2.0f, 4.0f));



	// A cool retro-style font
	//sf::Font font;
	//font.loadFromFile("fonts/DS-DIGI.ttf");

	//sf::Text text;
	//text.setFont(font);
	//text.setPosition(1, 1);
	//text.setString("Hallo");



	// Here is our clock for timing everything
	sf::Clock clock;
	
	window.setFramerateLimit(120);



	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);			
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		keyboardHandler.execute();

		
		// Update the delta time
		sf::Time deltaTime = clock.restart();

		ImGui::SFML::Update(window, deltaTime);


		ImGui::Begin("Properties"); // begin window

		ImGui::SetWindowSize(ImVec2(350.0f, 180.0f));
		// Background color edit
		if (ImGui::ColorEdit3("Background", color)) {
			// this code gets called if color value changes, so
			// the background color is upgraded automatically!
			bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
			bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
			bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
		}
		
		ImGui::SliderFloat("Wind", &wind, -0.5f, 0.5f, "%.4f");
		ImGui::SliderFloat("Gravity", &gravity, 0.001f, 1.0f, "%.4f");
		ImGui::Checkbox("Show Sun", &showSun);
		if (ImGui::Button("Drop Leaves")) {
			branch.dropLeaves();
		}
		if (ImGui::Button("Attach Leaves")) {
			branch.reset();
		}
		ImGui::End(); // end window



	
	
		sun.update(deltaTime);

		velocity_sun.x += s.x * deltaTime.asSeconds() * 300.0f;
		velocity_sun.y += s.y * deltaTime.asSeconds() * 300.0f;

		sf::Vector2f pos(std::sinf(static_cast<float>(M_2_PI) * (velocity_sun.x / 360.0f)) * 900.0f, 
			             std::sinf(static_cast<float>(M_2_PI) * (velocity_sun.y / 360.0f)) * 500.0f);


		

		
		window.clear(bgColor);

		if (showSun) {
			sf::Transform planetTransform = sf::Transform::Identity;
			planetTransform.translate(pos);
			planetTransform.scale(scale, scale, modes[0].width / 2, modes[0].height / 2);
			sun.draw(window, planetTransform);
		}
		
		
		sf::Vector2f windVec{ wind, 0.0 };
		branch.applyWind(&windVec);

		sf::Vector2f gravityVec{ 0.0f, gravity*0.1f };
		branch.applyGravity(&gravityVec);

		branch.update();
		
		
		branch.draw(window);
		

		ImGui::SFML::Render(window);
		
		window.display();
	}

	return 0;
}

