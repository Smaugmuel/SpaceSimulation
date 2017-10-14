#include "OrbitingPlanet.hpp"
#include "Projectile.hpp"
#include "SystemInformation.hpp"
#include "ViewHandler.hpp"
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
	// Most of this should probably be moved elsewhere since it's constant
	sf::CircleShape orbit = m_orbit;
	orbit.setPointCount(200);
	orbit.setRadius(GetOrbitRadius()*PX_PER_M);
	orbit.setOrigin(GetOrbitRadius()*PX_PER_M,GetOrbitRadius()*PX_PER_M);
	//orbit.setOutlineThickness(1.0f);
	orbit.setOutlineThickness(ViewHandler::Get()->GetViewSize().y * 0.001f);
	orbit.setFillColor(sf::Color::Transparent);
	orbit.setOutlineColor(sf::Color(255, 255, 255, 50));
	orbit.setPosition(WNDW * 0.5, WNDH * 0.5);


	target.draw(orbit, states);

	sf::CircleShape circle = m_circle;

	Vector2d screenPos = m_position * PX_PER_M;

	circle.setPosition(screenPos.x, WNDH - screenPos.y);

	target.draw(circle, states);

	m_planet_name->setScale(ViewHandler::Get()->GetViewSize() * 0.0004f);
	m_planet_name->setOrigin(
		m_planet_name->getLocalBounds().width / 2.0f,
		m_planet_name->getLocalBounds().height);
	m_planet_name->setPosition(screenPos.x, WNDH - screenPos.y - m_visual_radius - 20.0f * m_planet_name->getScale().y);


	target.draw(*m_planet_name, states);


	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		target.draw(*m_orbiting[i], states);
	}
}