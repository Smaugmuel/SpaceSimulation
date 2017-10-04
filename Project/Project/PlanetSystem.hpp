#ifndef PLANET_SYSTEM_HPP
#define PLANET_SYSTEM_HPP
#include <vector>
#include <SFML\Graphics\Drawable.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class Planet;

class PlanetSystem : public sf::Drawable
{
public:
	PlanetSystem();
	~PlanetSystem();

	void Update(float dt);

	void GetPlanets(std::vector<Planet*>& vec);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Planet* m_center;
};

#endif