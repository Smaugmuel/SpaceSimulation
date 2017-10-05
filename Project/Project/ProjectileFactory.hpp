#ifndef PROJECTILE_FACTORY_HPP
#define PROJECTILE_FACTORY_HPP

#include "Vector2.hpp"

class Projectile;
class Planet;

class ProjectileFactory
{
public:

	static Projectile* CreateProjectile(Planet* startPlanet);
};

#endif