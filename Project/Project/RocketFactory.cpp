#include "RocketFactory.hpp"
#include "Planet.hpp"
#include "Rocket.hpp"
#include "Input.hpp"
#include "SystemInformation.hpp"

Rocket * RocketFactory::CreateRocket(Planet * startPlanet)
{
	Vector2d mousePos = Input::Get()->MousePositionDouble() * (1.0 / PX_PER_M);
	Vector2d planetPos = startPlanet->GetPosition();
	Vector2d planetToMouse = mousePos - planetPos;
	Vector2d toPlanetSurface = planetToMouse.Normalized() * startPlanet->GetRadius();
	Vector2d startPosition = planetPos + toPlanetSurface;
	Vector2d startVelocity = planetToMouse.Normalized();
	Vector2d startAcceleration = Vector2d(0, 0);

	Rocket* r = new Rocket(startPosition, startVelocity, startAcceleration);
	r->SetVelocity(0.0, 0.0);

	return r;
}