#include "ProjectileFactory.hpp"
#include "Planet.hpp"
#include "Input.hpp"
#include "SystemInformation.hpp"
#include "ViewHandler.hpp"

#include <iostream>

Projectile * ProjectileFactory::CreateProjectile(Planet* startPlanet)
{
	Vector2d mousePos = Input::Get()->MousePositionDouble() * M_PER_PX;
	Vector2d planetPos = startPlanet->GetPosition();
	Vector2d planetToMouse = mousePos + ViewHandler::Get()->GetOriginOffset() - planetPos;
	Vector2d toPlanetSurface = planetToMouse.Normalized() * startPlanet->GetRadius();
	Vector2d startPosition = planetPos + toPlanetSurface;
	Vector2d startVelocity = startPlanet->GetVelocity() + planetToMouse * 5.0e-6;

	Projectile* p = new Projectile(startPosition, startVelocity);
	p->SetColor(sf::Color::Magenta);
	p->SetRadius(1.0f);

	return p;
}