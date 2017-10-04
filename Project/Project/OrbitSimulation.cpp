#include "OrbitSimulation.hpp"
#include "PlanetSystem.hpp"
#include "Projectile.hpp"
#include "OrbitingPlanet.hpp"

#include "SystemInformation.hpp"

#include "Input.hpp"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

OrbitSimulation::OrbitSimulation()
{
	m_planetSystem = new PlanetSystem;
	m_projectile = new Projectile(1.5858e12, WNDH * 0.5 / PX_PER_M - 7.0e9, 0.0 / PX_PER_M, 7399.79);
	m_projectile->SetVisualRadius(10);
	m_projectile->SetColor(sf::Color::Magenta);
}

OrbitSimulation::~OrbitSimulation()
{
	if (m_planetSystem)
	{
		delete m_planetSystem;
		m_planetSystem = nullptr;
	}
	if (m_projectile)
	{
		delete m_projectile;
		m_projectile = nullptr;
	}
}

void OrbitSimulation::Update(float dt)
{
	m_planetSystem->Update(3.15576e7 * YEARS_PER_S * dt);
	m_projectile->Update(3.15576e7 * YEARS_PER_S * dt);

	std::vector<Planet*> planets;
	m_planetSystem->GetPlanets(planets);

	m_projectile->SetAcceleration(0.0, 0.0);

	for (unsigned int i = 0; i < planets.size(); i++)
	{
		Vector2d direction = planets[i]->GetPosition() - m_projectile->GetPosition();
		double distance = direction.Length();
		direction.Normalize();

		double acceleration = 6.67408e-11 * planets[i]->GetMass() / std::pow(distance, 2);

		m_projectile->AddAcceleration(direction * acceleration);
	}
}

void OrbitSimulation::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_planetSystem, states);
	target.draw(*m_projectile, states);
}