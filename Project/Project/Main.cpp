#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <crtdbg.h>

#include "SystemInformation.hpp"
#include "OrbitSimulation.hpp"
#include "Input.hpp"
#include "ViewHandler.hpp"

#include "FPS_Counter.hpp"


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	sf::RenderWindow window(sf::VideoMode(WNDW, WNDH), "PhysicsTest", sf::Style::Default, settings);
	sf::View view = window.getDefaultView();
	sf::Clock time;
	sf::Event event;

	FPS_Counter fps(WNDW);

	window.setView(view);

	ViewHandler::Get()->SetWindow(&window);
	ViewHandler::Get()->SetView(&view);
	ViewHandler::Get()->SetEvent(&event);

	OrbitSimulation simulation;


	while (window.isOpen())
	{
		ViewHandler::Get()->UpdateWindow();

		float elapsedTime = 0.0f;

		while (elapsedTime < RENDER_COMPENSATION / FRAME_RATE)
		{
			float deltaTime = time.restart().asSeconds();
			elapsedTime += deltaTime;

			simulation.Update(deltaTime);
		}

		fps.Update();

		window.clear(sf::Color::Black);
		window.draw(simulation);
		window.draw(fps);
		window.display();
	}

	Input::Delete();
	ViewHandler::Delete();

	return 0;
}
