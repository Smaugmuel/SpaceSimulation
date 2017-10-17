#include "RocketFactory.hpp"
#include "Planet.hpp"
#include "Rocket.hpp"
#include "Input.hpp"
#include "SystemInformation.hpp"
#include "ViewHandler.hpp"

Rocket * RocketFactory::CreateRocket(Planet * startPlanet)
{
	Vector2d mousePos = Input::Get()->MousePositionDouble() * (1.0 / PX_PER_M);
	Vector2d planetPos = startPlanet->GetPosition();
	Vector2d planetToMouse = mousePos + ViewHandler::Get()->GetOriginOffset() - planetPos;
	Vector2d toPlanetSurface = planetToMouse.Normalized() * startPlanet->GetRadius();
	
	Vector2d startPosition = planetPos + toPlanetSurface;
	Vector2d startVelocity = startPlanet->GetVelocity();
	Vector2d startAcceleration = Vector2d(0, 0);
	Vector2d startDirection = toPlanetSurface.Normalized();

	Rocket* r = new Rocket(startPosition, startVelocity, startAcceleration, startDirection, startPlanet);

	return r;
}