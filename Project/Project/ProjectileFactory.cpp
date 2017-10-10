#include "ProjectileFactory.hpp"
#include "Planet.hpp"
#include "Input.hpp"
#include "SystemInformation.hpp"

#include <iostream>

Projectile * ProjectileFactory::CreateProjectile(Planet* startPlanet)
{
	Vector2d mousePos = Input::Get()->MousePositionDouble() * (1.0 / PX_PER_M);
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