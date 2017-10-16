#include "OrbitSimulation.hpp"

#include "PlanetSystem.hpp"
#include "OrbitingPlanet.hpp"

#include "Projectile.hpp"
#include "ProjectileFactory.hpp"

#include "Rocket.hpp"
#include "RocketFactory.hpp"

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
	m_planetSystem->Update(0.0f);				// "Initialize"

	m_paused = false;

	m_years_per_second = m_time_skip = EARTH_YEAR_PER_SECOND * 100;
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
	for (unsigned int i = 0; i < m_rockets.size(); i++)
	{
		if (m_rockets[i])
		{
			delete m_rockets[i];
			m_rockets[i] = nullptr;
		}
	}
}

void OrbitSimulation::Update(float dt)
{
	UpdateInput();
	detectCrash();
	if (!m_paused)
	{
		UpdateMovements(SECONDS_PER_EARTH_YEAR * m_years_per_second * dt);
		UpdateProjectileAcceleration();
	}
}

void OrbitSimulation::UpdateInput()
{
	Input::Get()->UpdateInput();

	if (Input::Get()->IsMousePressed(sf::Mouse::Button::Right))
	{
		m_rockets.push_back(RocketFactory::CreateRocket(m_planets[3]));
		//m_projectiles.push_back(ProjectileFactory::CreateProjectile(m_planets[3]));
	}
	//if (Input::Get()->IsMousePressed(sf::Mouse::Button::Right))
	//{
	//	m_rockets.push_back(RocketFactory::CreateRocket(m_planets[5]));
	//	//m_projectiles.push_back(ProjectileFactory::CreateProjectile(m_planets[5]));
	//}

	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Space))
	{
		m_paused = !m_paused;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Up))
	{
		m_years_per_second += m_time_skip;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Down))
	{
		m_years_per_second -= m_time_skip;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Right))
	{
		m_years_per_second += m_time_skip * 10.0;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Left))
	{
		m_years_per_second -= m_time_skip * 10.0;
	}

	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Num1))
	{
		for (unsigned int i = 0; i < m_rockets.size(); i++)
		{
			m_rockets[i]->ToggleThrust();
		}
	}
}

void OrbitSimulation::UpdateMovements(float dt)
{
	//m_planetSystem->Update(dt);

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->Update(dt);
	}

	for (unsigned int i = 0; i < m_rockets.size(); i++)
	{
		m_rockets[i]->Update(dt);
	}
}

void OrbitSimulation::UpdateProjectileAcceleration()
{
	//std::vector<Planet*> planets;
	//m_planetSystem->GetPlanets(planets);

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		for (unsigned int j = 0; j < m_planets.size(); j++)
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_projectiles[i]->GetPosition();
			double distance = direction.Length();
			direction.Normalize();

			double acceleration = 6.67408e-11 * m_planets[j]->GetMass() / std::pow(distance, 2);

			m_projectiles[i]->AddAcceleration(direction * acceleration);
		}
	}

	for (unsigned int i = 0; i < m_rockets.size(); i++)
	{
		for (unsigned int j = 0; j < m_planets.size(); j++)
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_rockets[i]->GetPosition();
			double distance = direction.Length();
			direction.Normalize();

			double acceleration = 6.67408e-11 * m_planets[j]->GetMass() / std::pow(distance, 2);

			m_rockets[i]->AddAcceleration(direction * acceleration);
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

	for (unsigned int i = 0; i < m_rockets.size(); i++)
	{
		target.draw(*m_rockets[i], states);
	}
}

void OrbitSimulation::detectCrash() 
{
	for (unsigned int i = 0; i < m_projectiles.size(); i++) 
	{
		for (unsigned int j=0; j < m_planets.size(); j++) 
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_projectiles[i]->GetPosition();
			double distance = direction.Length();

			if (distance < (m_planets[j]->GetRadius()))
			{
			
				m_projectiles[i]->SetIsCrashed(true);

				//delete m_projectiles[i];
				//m_projectiles.erase(m_projectiles.begin() + i);
			
				//i--;
			}
		}
	}

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i]->GetIsCrashed())
		{
			m_projectiles[i]->SetAcceleration(0, 0);
			m_projectiles[i]->SetVelocity(0, 0);

			m_projectiles[i]->SetColor(sf::Color::Red);
			m_projectiles[i]->SetRadius(m_projectiles[i]->GetRadius() + 2.0e4);
			if (m_projectiles[i]->GetRadius() > 5.0e7)
			{
				delete m_projectiles[i];
				m_projectiles.erase(m_projectiles.begin() + i);
				i--;
			}
		}
	}
}