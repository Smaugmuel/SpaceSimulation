#include "ProjectileFactory.hpp"
#include "Planet.hpp"
#include "Input.hpp"
#include "SystemInformation.hpp"

#include <iostream>

Projectile * ProjectileFactory::CreateProjectile(Planet* startPlanet)
{
	//Vector2d mousePos = Input::Get()->MousePosition() * (1.0 / PX_PER_M);

	// DO NOT REMOVE! Have not been able to figure out what is causing this bug yet.
	/*
	Vector2d pos = Input::Get()->MousePositionDouble(); // Vector2d = const Vector2d

	std::cout << Input::Get()->MousePositionDouble().x << ", ";			// CORRECT VALUE
	std::cout << Input::Get()->MousePositionDouble().y << std::endl;	// CORRECT VALUE

	std::cout << pos.x << ", ";				// CORRECT VALUE
	std::cout << pos.y << std::endl;		// INCORRECT VALUE!?!?!?

	std::cout << std::endl;
	*/

	Vector2d mousePos;
	mousePos.x = Input::Get()->MousePositionDouble().x;
	mousePos.y = Input::Get()->MousePositionDouble().y;
	mousePos *= (1 / PX_PER_M);


	Vector2d planetPos = startPlanet->GetPosition();
	Vector2d planetToMouse = mousePos - planetPos;
	Vector2d toPlanetSurface = planetToMouse.Normalized() * startPlanet->GetRadius();
	Vector2d startPosition = planetPos + toPlanetSurface;
	Vector2d startVelocity = planetToMouse * 5.0e-7;

	Projectile* p = new Projectile(startPosition, startVelocity);
	p->SetColor(sf::Color::Magenta);
	p->SetRadius(2.0e6);

	return p;
}