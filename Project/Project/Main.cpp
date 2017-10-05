#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include <crtdbg.h>

#include "SystemInformation.hpp"
#include "OrbitSimulation.hpp"

//todo: move this after merge with FiringProjectile branch
#include <SFML\Window\Mouse.hpp>

//for debugging
#include <iostream>

void ZoomInOnMouse(sf::RenderWindow& window, sf::Event& event, sf::View& view);

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::RenderWindow window(sf::VideoMode(WNDW, WNDH), "PhysicsTest");
	sf::View view = window.getDefaultView();
	sf::Clock time;

	OrbitSimulation simulation;

	window.setView(view);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseWheelMoved:
				ZoomInOnMouse(window, event, view);
				break;

			case sf::Event::Resized: // window resizing currently resets the zoom
				{
					sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
					view = sf::View(visibleArea);
					window.setView(view);
				}
				break;
			default:
				break;
			}
		}

		float elapsedTime = 0.0f;


		while (elapsedTime < RENDER_COMPENSATION / FRAME_RATE)
		{
			float deltaTime = time.restart().asSeconds();
			elapsedTime += deltaTime;

			simulation.Update(deltaTime);
		}

		window.clear(sf::Color::Black);
		window.draw(simulation);
		window.display();
	}

	return 0;
}

void ZoomInOnMouse(sf::RenderWindow& window, sf::Event& event, sf::View& view)
{
	//std::cout << event.mouseWheel.delta << std::endl;

	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixelPos) };
	view.zoom(1.0f - event.mouseWheel.delta * 0.1f);
	window.setView(view);

	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixelPos) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}