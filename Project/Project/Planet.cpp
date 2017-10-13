#include "Planet.hpp"
#include "OrbitingPlanet.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include "SystemInformation.hpp"

Planet::Planet()
{
}

Planet::~Planet()
{
	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		if (m_orbiting[i])
		{
			delete m_orbiting[i];
			m_orbiting[i] = nullptr;
		}
	}
}

void Planet::Update(float dt)
{
	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		m_orbiting[i]->Update(dt);
	}
}

OrbitingPlanet* Planet::CreateOrbitingPlanet()
{
	m_orbiting.push_back(new OrbitingPlanet(this));
	return m_orbiting.back();
}

void Planet::GetPlanets(std::vector<Planet*>& vec)
{
	vec.push_back(this);
	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		m_orbiting[i]->GetPlanets(vec);
	}
}

OrbitingPlanet * Planet::GetOrbitingPlanet(unsigned int i)
{
	return i < m_orbiting.size() ? m_orbiting[i] : nullptr;
}

void Planet::draw(sf::RenderTarget & target, sf::RenderStates states) const 
{
	sf::CircleShape circle = m_circle;

	Vector2d screenPos = m_position * PX_PER_M;

	circle.setPosition(screenPos.x, WNDH - screenPos.y);

	target.draw(circle, states);

	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		target.draw(*m_orbiting[i], states);
	}
}