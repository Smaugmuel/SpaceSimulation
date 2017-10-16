#include "Rocket.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include "SystemInformation.hpp"
#include "ViewHandler.hpp"

Rocket::Rocket()
	: Rocket::Rocket(0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
{
}

Rocket::Rocket(Vector2d position, Vector2d velocity, Vector2d acceleration)
	: Rocket::Rocket(position.x, position.y, velocity.x, velocity.y, acceleration.x, acceleration.y)
{
}

Rocket::Rocket(double x, double y, double vx, double vy, double ax, double ay)
{
	m_position = Vector2d(x, y);
	m_velocity = Vector2d(vx, vy);
	m_acceleration = Vector2d(ax, ay);

	m_payloadMass = 140000;
	AddStep(2290000, 130000, 263, 165);

	m_isThrusting = true;

	m_triangle.setPointCount(3);
	m_triangle.setRadius(2.0f);
	m_triangle.setOrigin(2.0f, 0.0f);
	m_triangle.setPosition(x * PX_PER_M, y * PX_PER_M);
	m_triangle.setScale(1.0f, 2.0f);
	m_triangle.setFillColor(sf::Color::Magenta);

	UpdateRotation();
}

Rocket::~Rocket()
{
	while (m_steps.size())
	{
		delete m_steps.front();
		m_steps.pop_front();
	}
}

void Rocket::Update(float dt)
{
	m_position += m_velocity * dt;
	m_velocity += m_acceleration * dt;

	m_acceleration = Vector2d(0.0, 0.0);

	UpdateThrust(dt);
	UpdateRotation();
}

void Rocket::UpdateThrust(float dt)
{
	return;
	if (m_steps.empty())
	{
		m_isThrusting = false;
	}

	if (m_isThrusting)
	{
		Step* step = m_steps.front();

		// Calculate the mass of fuel used during this iteration
		double fuelMassLoss = step->GetFuelMassLossPerSecond() * dt;
		double actualFuelMassLoss = std::min(step->GetFuelMass(), fuelMassLoss);

		// Acceleration based on mass before fuel loss. Multiplication by dt should occur only in Update()
		double acceleration = actualFuelMassLoss * step->GetExhaustVelocity() / GetTotalMass() / dt;
		double rotation = /*m_rotation*/45 * 3.1415927 / 180;
		m_acceleration += Vector2d(cos(rotation), sin(rotation)) * acceleration;

		// Decrease mass, or remove step
		if (actualFuelMassLoss < step->GetFuelMass())
		{
			step->AddFuelMass(-actualFuelMassLoss);
		}
		else
		{
			delete step;
			m_steps.pop_front();
		}
	}
}

void Rocket::UpdateRotation()
{
	Vector2d e_v = m_velocity.Normalized();

	if (e_v.LengthSquared() > 0.0)
	{
		int positive = (float)e_v.y / std::fabsf((float)e_v.y);
		m_rotation = positive * std::acosf(e_v.x) * 180 / 3.1415927f;
	}
	
	m_triangle.setRotation(90 - m_rotation);
}

void Rocket::SetPosition(const Vector2d & position)
{
	m_position = position;
}

void Rocket::SetPosition(double x, double y)
{
	m_position.x = x;
	m_position.y = y;
}

void Rocket::SetVelocity(const Vector2d & velocity)
{
	m_velocity = velocity;
}

void Rocket::SetVelocity(double x, double y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

void Rocket::SetAcceleration(const Vector2d & acceleration)
{
	m_acceleration = acceleration;
}

void Rocket::SetAcceleration(double x, double y)
{
	m_acceleration.x = x;
	m_acceleration.y = y;
}

void Rocket::SetRotation(double rotation)
{
	m_rotation = rotation;
	UpdateRotation();
}

void Rocket::SetPayloadMass(double mass)
{
	m_payloadMass = mass;
}

void Rocket::ToggleThrust()
{
	m_isThrusting = !m_isThrusting;
}

void Rocket::AddPosition(const Vector2d & v)
{
	m_position += v;
}

void Rocket::AddPosition(double x, double y)
{
	m_position.x += x;
	m_position.y += y;
}

void Rocket::AddVelocity(const Vector2d & v)
{
	m_velocity += v;
}

void Rocket::AddVelocity(double x, double y)
{
	m_velocity.x += x;
	m_velocity.y += y;
}

void Rocket::AddAcceleration(const Vector2d & v)
{
	m_acceleration += v;
}

void Rocket::AddAcceleration(double x, double y)
{
	m_acceleration.x += x;
	m_acceleration.y += y;
}

void Rocket::AddStep(double grossMass, double emptyMass, double specificImpulse, double burnTime)
{
	Step* step = new Step;

	step->SetFuelMass(grossMass - emptyMass);
	step->SetHullMass(emptyMass);
	step->SetExhaustVelocity(specificImpulse * 9.82);
	step->SetFuelMassLossPerSecond(step->GetFuelMass() / burnTime);

	m_steps.push_back(step);
}

const Vector2d & Rocket::GetPosition() const
{
	return m_position;
}

const Vector2d & Rocket::GetVelocity() const
{
	return m_velocity;
}

const Vector2d & Rocket::GetAcceleration() const
{
	return m_acceleration;
}

const double & Rocket::GetPayloadMass() const
{
	return m_payloadMass;
}

const double & Rocket::GetTotalMass() const
{
	double mass = m_payloadMass;

	for (auto& step : m_steps)
	{
		mass += step->GetTotalMass();
	}
	return mass;
}

const bool & Rocket::GetIsThrusting() const
{
	return m_isThrusting;
}

void Rocket::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::CircleShape triangle = m_triangle;

	Vector2f screenPos = m_position * PX_PER_M;
	triangle.setPosition(screenPos.x, WNDH - screenPos.y);
	triangle.setScale(sf::Vector2f(1.0f, 2.0f) * (ViewHandler::Get()->GetViewSize().x * 0.002f));

	target.draw(triangle, states);
}



Step::Step()
{
	/*
	First Step
	-----------------------------------

	Total mass 
	Gross mass of first step	m = 2,290,000 kg
	Thrust at sea level			35,100,000 N
	Specific impulse			263 s
	Burn time					165 s

	
	*/

	// Gross mass: 2290000

	// Approximate values taken from Saturn V's first step
	m_hullMass = 100000;
	m_fuelMass = 2000000;
	m_fuelMassLossPerSecond = 1000000;
	m_exhaustVelocity = 2000;
}

Step::~Step()
{
}

void Step::Update(float dt)
{
}

void Step::SetHullMass(double mass)
{
	m_hullMass = mass;
}

void Step::SetFuelMass(double mass)
{
	m_fuelMass = mass;
}

void Step::SetExhaustVelocity(double ve)
{
	m_exhaustVelocity = ve;
}

void Step::SetFuelMassLossPerSecond(double m)
{
	m_fuelMassLossPerSecond = m;
}

void Step::AddFuelMass(double mass)
{
	m_fuelMass += mass;
}

const double& Step::GetHullMass() const
{
	return m_hullMass;
}

const double& Step::GetFuelMass() const
{
	return m_fuelMass;
}

const double& Step::GetTotalMass() const
{
	return m_hullMass + m_fuelMass;
}

const double& Step::GetExhaustVelocity() const
{
	return m_exhaustVelocity;
}

const double& Step::GetFuelMassLossPerSecond() const
{
	return m_fuelMassLossPerSecond;
}