#include "RocketFactory.hpp"
#include "Planet.hpp"
#include "Rocket.hpp"
#include "Input.hpp"
#include "SystemInformation.hpp"

Rocket * RocketFactory::CreateRocket(Planet * startPlanet)
{
	Vector2d mousePos = Input::Get()->MousePositionDouble() * (1.0 / PX_PER_M);
	Vector2d rocketPos = startPlanet->GetPosition();
	Vector2d rocketToMouse = mousePos - rocketPos;
	Vector2d toPlanetSurface = rocketToMouse.Normalized() * startPlanet->GetRadius();
	Vector2d startPosition = rocketPos + toPlanetSurface;
	Vector2d startVelocity = rocketToMouse * 5.0e-7;

	Rocket* r = new Rocket(startPosition, startVelocity);

	return r;
}