#ifndef ROCKET_FACTORY_HPP
#define ROCKET_FACTORY_HPP

#include "Vector2.hpp"

class Rocket;
class Planet;

class RocketFactory
{
public:

	static Rocket* CreateRocket(Planet* startPlanet);
};

#endif