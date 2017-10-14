#include "PlanetSystem.hpp"
#include "Planet.hpp"
#include "OrbitingPlanet.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include "SystemInformation.hpp"

PlanetSystem::PlanetSystem()
{
	// Sun
	m_center = new Planet;
	m_center->SetPosition(WNDW * 0.5 / PX_PER_M, WNDH * 0.5 / PX_PER_M);
	m_center->SetColor(sf::Color(255, 255, 0));
	m_center->SetRadius(6.957e8);
	m_center->SetMass(1.989e30);
	m_center->SetName("Sun");

	OrbitingPlanet* planet;

	// Mercury
	planet = m_center->CreateOrbitingPlanet();
	planet->SetColor(sf::Color(169, 169, 169));
	planet->SetRadius(2.4397e6);
	planet->SetMass(3.285e23);
	planet->SetOrbitRadius(5.7e10);
	planet->SetOrbitTime(7.6032e6);
	planet->SetName("Mercury");

	// Venus
	planet = m_center->CreateOrbitingPlanet();
	planet->SetColor(sf::Color(255, 150, 71));
	planet->SetRadius(6.052e6);
	planet->SetMass(4.86732e24);
	planet->SetOrbitRadius(1.082e11);
	planet->SetOrbitTime(1.941408e7);
	planet->SetName("Venus");

	// Earth
	planet = m_center->CreateOrbitingPlanet();
	planet->SetColor(sf::Color(30, 144, 255));
	planet->SetRadius(6.371e6);
	planet->SetMass(5.972e24);
	planet->SetOrbitRadius(1.496e11);
	planet->SetOrbitTime(3.15576e7);
	planet->SetName("Earth");

	// Mars
	planet = m_center->CreateOrbitingPlanet();
	planet->SetColor(sf::Color(255, 69, 0));
	planet->SetRadius(3.39e6);
	planet->SetMass(6.41693e23);
	planet->SetOrbitRadius(2.279e11);
	planet->SetOrbitTime(5.93568e7);
	planet->SetName("Mars");

	// Jupiter
	planet = m_center->CreateOrbitingPlanet();
	planet->SetColor(sf::Color(205, 133, 63));
	planet->SetRadius(6.9911e7);
	planet->SetMass(1.898e27);
	planet->SetOrbitRadius(7.785e11);
	planet->SetOrbitTime(3.742848e8);
	planet->SetName("Jupiter");
}

PlanetSystem::~PlanetSystem()
{
	if (m_center)
	{
		delete m_center;
		m_center = nullptr;
	}
}

void PlanetSystem::Update(float dt)
{
	m_center->Update(dt);
}

void PlanetSystem::GetPlanets(std::vector<Planet*>& vec)
{
	m_center->GetPlanets(vec);
}

void PlanetSystem::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_center, states);
}