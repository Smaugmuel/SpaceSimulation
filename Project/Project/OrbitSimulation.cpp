#include "OrbitSimulation.hpp"

#include "PlanetSystem.hpp"
#include "OrbitingPlanet.hpp"

#include "Projectile.hpp"
#include "ProjectileFactory.hpp"

#include "Rocket.hpp"
#include "RocketFactory.hpp"

#include "SystemInformation.hpp"

#include "Input.hpp"
#include "ViewHandler.hpp"
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

#include <sstream>
#include <iostream>

OrbitSimulation::OrbitSimulation()
{
	m_planetSystem = new PlanetSystem;
	m_planetSystem->GetPlanets(m_planets);		// Bad design, see header file
	//m_planetSystem->Update(0.0f);				// "Initialize"

	m_paused = false;

	m_years_per_second = m_time_skip = EARTH_YEAR_PER_SECOND * 10;

	m_font = std::make_unique<sf::Font>();
	m_hud_text = std::make_unique<sf::Text>();

	m_font->loadFromFile("../Assets/cour.ttf");
	m_hud_text->setFont(*m_font);
	m_hud_text->setPosition(0.0f, 0.0f);
	m_hud_text->setColor(sf::Color::White);
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
		UpdateAccelerations(SECONDS_PER_EARTH_YEAR * m_years_per_second * dt);
	}
}

void OrbitSimulation::UpdateInput()
{
	Input::Get()->UpdateInput();

	if (Input::Get()->IsMousePressed(sf::Mouse::Button::Left))
	{
		m_rockets.push_back(RocketFactory::CreateRocket(m_planets[3]));
		//m_projectiles.push_back(ProjectileFactory::CreateProjectile(m_planets[3]));
	}
	if (Input::Get()->IsMousePressed(sf::Mouse::Button::Right))
	{
		//m_rockets.push_back(RocketFactory::CreateRocket(m_planets[3]));
		m_projectiles.push_back(ProjectileFactory::CreateProjectile(m_planets[3]));
	}

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
		m_years_per_second += m_time_skip * 50.0;
	}
	if (Input::Get()->IsKeyPressed(sf::Keyboard::Key::Left))
	{
		m_years_per_second -= m_time_skip * 50.0;
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
	m_planetSystem->Update(dt);

	for (unsigned int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->Update(dt);
	}

	for (unsigned int i = 0; i < m_rockets.size(); i++)
	{
		m_rockets[i]->Update(dt);
	}
}

void OrbitSimulation::UpdateAccelerations(float dt)
{
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

	for (unsigned int i = 0; i < m_rockets.size(); i++)
	{
		m_rockets[i]->SetAcceleration(0.0, 0.0);

		for (unsigned int j = 0; j < m_planets.size(); j++)
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_rockets[i]->GetPosition();
			double distance = direction.Length();
			direction.Normalize();

			double acceleration = 6.67408e-11 * m_planets[j]->GetMass() / std::pow(distance, 2);

			m_rockets[i]->AddAcceleration(direction * acceleration);
		}

		m_rockets[i]->UpdateThrust(dt);
		m_rockets[i]->UpdateRotation();
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

	// Draw hud
	std::ostringstream ss;
	ss << m_years_per_second * SECONDS_PER_EARTH_YEAR << "x real time\n";
	ss << m_years_per_second * SECONDS_PER_EARTH_YEAR / 3600 << " hours/second";
	m_hud_text->setString(ss.str());

	ViewHandler::Get()->SetViewToWindow(ViewHandler::Get()->m_hud_view);
	target.draw(*m_hud_text, states);
	ViewHandler::Get()->SetViewToWindow(ViewHandler::Get()->m_view);
}

void OrbitSimulation::detectCrash() 
{
	for (unsigned int j = 0; j < m_planets.size(); j++)
	{
		for (unsigned int i = 0; i < m_projectiles.size(); i++)
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_projectiles[i]->GetPosition();

			if (direction.Length() < m_planets[j]->GetRadius())
			{

				//m_projectiles[i]->SetIsCrashed(true);
				delete m_projectiles[i];
				m_projectiles.erase(m_projectiles.begin() + i);
				i--;

				//delete m_projectiles[i];
				//m_projectiles.erase(m_projectiles.begin() + i);

				//i--;
			}
		}
	}

	//for (unsigned int i = 0; i < m_projectiles.size(); i++)
	//{
	//	if (m_projectiles[i]->GetIsCrashed())
	//	{
	//		m_projectiles[i]->SetAcceleration(0, 0);
	//		m_projectiles[i]->SetVelocity(0, 0);

	//		m_projectiles[i]->SetColor(sf::Color::Red);
	//		m_projectiles[i]->SetRadius(m_projectiles[i]->GetRadius() + 2.0e4);
	//		if (m_projectiles[i]->GetRadius() > 5.0e7)
	//		{
	//			delete m_projectiles[i];
	//			m_projectiles.erase(m_projectiles.begin() + i);
	//			i--;
	//		}
	//	}
	//}


	for (unsigned int j = 0; j < m_planets.size(); j++)
	{
		for (unsigned int i = 0; i < m_rockets.size(); i++)
		{
			Vector2d direction = m_planets[j]->GetPosition() - m_rockets[i]->GetPosition();
			double distance = direction.Length();

			if (distance < (m_planets[j]->GetRadius()) - 30000)
			{
				//m_rockets[i]->SetIsCrashed(true);

				delete m_rockets[i];
				m_rockets.erase(m_rockets.begin() + i);
				i--;

				//delete m_projectiles[i];
				//m_projectiles.erase(m_projectiles.begin() + i);

				//i--;
			}
		}
	}
	//for (unsigned int i = 0; i < m_rockets.size(); i++)
	//{
	//	if (m_rockets[i]->GetCrashed())
	//	{
	//		m_rockets[i]->SetAcceleration(0, 0);
	//		m_rockets[i]->SetVelocity(0, 0);

	//		m_rockets[i]->SetColor(sf::Color::Red);
	//		m_rockets[i]->SetRadius(m_rockets[i]->GetRadius() + 2.0e4);
	//		/*if (m_rockets[i]->GetRadius() > 5.0e7)
	//		{*/
	//			delete m_rockets[i];
	//			m_rockets.erase(m_rockets.begin() + i);
	//			i--;
	//		//}
	//	}
	//}
}