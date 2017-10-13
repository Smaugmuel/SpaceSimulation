#ifndef PLANET_HPP
#define PLANET_HPP
#include <vector>
#include "Projectile.hpp"

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class OrbitingPlanet;

class Planet : public Projectile
{
public:
	Planet();
	virtual ~Planet();

	void Update(float dt);

	OrbitingPlanet* CreateOrbitingPlanet();

	void GetPlanets(std::vector<Planet*>& vec);
	OrbitingPlanet* GetOrbitingPlanet(unsigned int i);

protected:
	std::vector<OrbitingPlanet*> m_orbiting;
	std::string name;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif