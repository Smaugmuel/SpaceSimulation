#ifndef ORBIT_SIMULATION_HPP
#define ORBIT_SIMULATION_HPP
#include <SFML\Graphics\Drawable.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
}
class PlanetSystem;
class Projectile;

class OrbitSimulation : public sf::Drawable
{
public:
	OrbitSimulation();
	~OrbitSimulation();

	void Update(float dt);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	PlanetSystem* m_planetSystem;
	Projectile* m_projectile;
};

#endif