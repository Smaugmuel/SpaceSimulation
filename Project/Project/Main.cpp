#include <SFML\Window\Event.hpp>
#include <crtdbg.h>

#include "SystemInformation.hpp"
#include "OrbitSimulation.hpp"
#include "Input.hpp"


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	sf::RenderWindow window(sf::VideoMode(WNDW, WNDH), "PhysicsTest");
	sf::View view = window.getDefaultView();
	sf::Clock time;
	sf::Event event;

	window.setView(view);

	Input::Get()->SetWindow(&window);
	Input::Get()->SetView(&view);
	Input::Get()->SetEvent(&event);

	OrbitSimulation simulation;


	while (window.isOpen())
	{
		Input::Get()->UpdateWindow();

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

	Input::Delete();

	return 0;
}
