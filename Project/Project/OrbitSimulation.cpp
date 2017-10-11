#include "OrbitSimulation.hpp"
#include "PlanetSystem.hpp"
#include "Projectile.hpp"
#include "OrbitingPlanet.hpp"
#include "ProjectileFactory.hpp"

#include "SystemInformation.hpp"

#include "Input.hpp"
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

#include <iostream>

OrbitSimulation::OrbitSimulation()
{
	m_planetSystem = new PlanetSystem;
	m_planetSystem->GetPlanets(m_planets);		// Bad design, see header file

	//m_projectile = new Projectile(1.5858e12, WNDH * 0.5 / PX_PER_M - 7.0e9, 0.0 / PX_PER_M, 7399.79);
	//m_projectile->SetVisualRadius(10);
	//m_projectile->SetColor(sf::Color::Magenta);

	m_paused = false;
	m_years_per_second = 0.1;
}

OrbitSimulation::~OrbitSimulation()
{
	if (m_planetSystem)
	{
		delete m_planetSystem;
		m_planetSystem = nullptr;
	}
	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i])
		{
			delete m_projectiles[i];
			m_projectiles[i] = nullptr;
		}
	}
}

void OrbitSimulation::Update(float dt)
{
	UpdateInput();
	detectCrash();
	if (!m_paused)
	{
		double secondsInOneEarthYear = 3.15576e7;

		UpdateMovements(secondsInOneEarthYear * m_years_per_second * dt);

		UpdateProjectileAcceleration();
	}
}

void OrbitSimulation::UpdateInput()
{
	Input::Get()->UpdateInput();

	if (Input::Get()->IsMousePressed(sf::Mouse::Button::Left))
	{
		m_projectiles.push_back(ProjectileFactory::CreateProjectile(m_planets[3]));
	}
	if (Input::Get()->IsMousePressed(sf::Mouse::Button::Right))
	{
		m_projectiles.push_back(ProjectileFactory::CreateProjectile(m_planets[5]));
	}

	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Space))
	{
		m_paused = !m_paused;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Up))
	{
		m_years_per_second += 0.1;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Down))
	{
		m_years_per_second -= 0.1;
	}
}

void OrbitSimulation::UpdateMovements(float dt)
{
	m_planetSystem->Update(dt);

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->Update(dt);
	}
}

void OrbitSimulation::UpdateProjectileAcceleration()
{
	//std::vector<Planet*> planets;
	//m_planetSystem->GetPlanets(planets);

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->SetAcceleration(0.0, 0.0);

		for (unsigned int j = 0; j < m_planets.size(); j++)
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_projectiles[i]->GetPosition();
			double distance = direction.Length();
			direction.Normalize();

			double acceleration = 6.67408e-11 * m_planets[j]->GetMass() / std::pow(distance, 2);

			m_projectiles[i]->AddAcceleration(direction * acceleration);
		}
	}
}

void OrbitSimulation::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_planetSystem, states);

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		target.draw(*m_projectiles[i], states);
	}
}

void OrbitSimulation::detectCrash() 
{
	
	for (unsigned int i = 0; i < m_projectiles.size(); i++) 
	{
		bool crashDetected = false;
		for (unsigned int j=0; j < m_planets.size() && !crashDetected ; j++) 
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_projectiles[i]->GetPosition();
			double distance = direction.Length();

			if (distance < (m_planets[j]->GetRadius()))
			{
				crashDetected = true;
				
				delete m_projectiles[i];
				m_projectiles.erase(m_projectiles.begin() + i);
			
				i--;
			}
		}
		if (crashDetected)
		{
			m_paused = true;
		}
	}

		
}