#ifndef ORBIT_SIMULATION_HPP
#define ORBIT_SIMULATION_HPP
#include <SFML\Graphics\Drawable.hpp>
#include <vector>
#include <memory>

namespace sf
{
	class RenderTarget;
	class RenderStates;
	class Font;
	class Text;
}
class PlanetSystem;
class Projectile;
class Planet;
class Rocket;

class OrbitSimulation : public sf::Drawable
{
public:
	OrbitSimulation();
	~OrbitSimulation();

	void Update(float dt);

private:
	void UpdateInput();
	void UpdateMovements(float dt);
	void UpdateAccelerations(float dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void detectCrash();
	PlanetSystem* m_planetSystem;
	
	std::vector<Planet*> m_planets;			// Bad design of storing in multiple places, but will work for now
	std::vector<Projectile*> m_projectiles;
	std::vector<Rocket*> m_rockets;

	bool m_paused;
	double m_years_per_second;
	double m_time_skip;

	std::unique_ptr<sf::Text> m_hud_text;
	std::unique_ptr<sf::Font> m_font;
};

#endif