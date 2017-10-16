#include "Planet.hpp"
#include "OrbitingPlanet.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Font.hpp>
#include "SystemInformation.hpp"
#include "Input.hpp"
#include "ViewHandler.hpp"

Planet::Planet()
{
	m_font = std::make_unique<sf::Font>();
	m_planet_name = std::make_unique<sf::Text>();

	m_font->loadFromFile("../Assets/Arcon-Regular.otf");
	m_planet_name->setFont(*m_font);
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

void Planet::SetName(const std::string &name)
{
	m_planet_name->setString(name);
	m_planet_name->setColor(sf::Color::White);
}

void Planet::SetTextSize(unsigned int size)
{
	m_planet_name->setCharacterSize(size);
}

void Planet::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::CircleShape circle = m_circle;

	Vector2d screenPos = (m_position - ViewHandler::Get()->GetOriginOffset())* PX_PER_M;

	circle.setPosition(screenPos.x, WNDH - screenPos.y);

	m_planet_name->setScale(ViewHandler::Get()->GetViewSize() * 0.0004f);
	m_planet_name->setOrigin(
		m_planet_name->getLocalBounds().width / 2.0f, 
		m_planet_name->getLocalBounds().height);
	m_planet_name->setPosition(screenPos.x, WNDH - screenPos.y - m_visual_radius - 20.0f * m_planet_name->getScale().y);

	target.draw(circle, states);
	target.draw(*m_planet_name, states);

	for (unsigned int i = 0; i < m_orbiting.size(); i++)
	{
		target.draw(*m_orbiting[i], states);
	}
}