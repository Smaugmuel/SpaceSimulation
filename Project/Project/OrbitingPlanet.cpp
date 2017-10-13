#include "OrbitingPlanet.hpp"
#include "Projectile.hpp"
#include "SystemInformation.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>


OrbitingPlanet::OrbitingPlanet(Planet* orbitedPlanet) :
	Planet(),
	m_orbitedPlanet(orbitedPlanet),
	m_orbitAngle(0.0),
	m_orbitRadius(200.0)
{
}

OrbitingPlanet::~OrbitingPlanet()
{
}

void OrbitingPlanet::Update(float dt)
{
	m_orbitAngle += 2 * 3.1415927 / m_orbitTime * dt;

	// Set position based on angle
	Vector2d difference = Vector2d(cosf(m_orbitAngle), sinf(m_orbitAngle)) * m_orbitRadius;
	SetPosition(m_orbitedPlanet->GetPosition() + difference);

	// Update orbiting planets
	Planet::Update(dt);
}

void OrbitingPlanet::SetOrbitAngle(double startAngle)
{
	m_orbitAngle = startAngle;
}

void OrbitingPlanet::SetOrbitRadius(double radius)
{
	m_orbitRadius = radius;
}

void OrbitingPlanet::SetOrbitTime(double time)
{
	m_orbitTime = time;
}
double OrbitingPlanet::GetOrbitRadius()const
{
	return m_orbitRadius;
}

void OrbitingPlanet::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::CircleShape orbit = m_orbit;
	orbit.setRadius(GetOrbitRadius()*PX_PER_M);
	orbit.setOrigin(GetOrbitRadius()*PX_PER_M,GetOrbitRadius()*PX_PER_M);
	orbit.setOutlineThickness(50.0f);
	orbit.setOutlineColor(sf::Color::White);
	orbit.setPosition(WNDW * 0.5 / PX_PER_M, WNDH * 0.5 / PX_PER_M);

	target.draw(orbit, states);

	sf::CircleShape circle = m_circle;

	Vector2d screenPos = m_position * PX_PER_M;

	circle.setPosition(screenPos.x, WNDH - screenPos.y);

	target.draw(circle, states);

	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		target.draw(*m_orbiting[i], states);
	}
}