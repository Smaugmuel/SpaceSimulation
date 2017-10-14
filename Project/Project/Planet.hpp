#ifndef PLANET_HPP
#define PLANET_HPP
#include <vector>
#include "Projectile.hpp"
#include <memory>

#include <SFML/Graphics.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
	class Font;
	class Text;
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
	void SetName(const std::string &name);
	void Planet::SetTextSize(unsigned int size);
	OrbitingPlanet* GetOrbitingPlanet(unsigned int i);

protected:
	std::vector<OrbitingPlanet*> m_orbiting;
	std::unique_ptr<sf::Text> m_planet_name;
	std::unique_ptr<sf::Font> m_font;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif