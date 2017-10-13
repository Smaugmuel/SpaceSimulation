#include "Rocket.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include "SystemInformation.hpp"

Rocket::Rocket()
	: Rocket::Rocket(0.0, 0.0, 0.0, 0.0)
{
}

Rocket::Rocket(Vector2d position, Vector2d velocity)
	: Rocket::Rocket(position.x, position.y, velocity.x, velocity.y)
{
}

Rocket::Rocket(double x, double y, double vx, double vy)
{
	m_position = Vector2d(x, y);
	m_velocity = Vector2d(vx, vy);
	m_acceleration = Vector2d(0.0, 0.0);

	m_payloadMass = 10000.0;		// 10 tonne
	m_steps.push_back(new Step);

	m_triangle.setPointCount(3);
	m_triangle.setRadius(7.0f);
	m_triangle.setOrigin(10.0f, 10.0f);
	m_triangle.setPosition(x * PX_PER_M, y * PX_PER_M);
	m_triangle.setScale(1.0f, 2.0f);
	m_triangle.setFillColor(sf::Color::Magenta);
}

Rocket::~Rocket()
{
	for (unsigned int i = 0; i < m_steps.size(); i++)
	{
		delete m_steps[i];
		m_steps[i] = nullptr;
	}
}

void Rocket::Update(float dt)
{
	m_position += m_velocity * dt;
	m_velocity += m_acceleration * dt;

	Vector2d e_v = m_velocity.Normalized();
	int positive = (float)e_v.y / std::fabsf((float)e_v.y);
	float rotation = positive * std::acosf(e_v.x) * 180 / 3.1415927f;

	m_triangle.setRotation(90 - rotation);
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

void Rocket::SetPayloadMass(double mass)
{
	m_payloadMass = mass;
}

void Rocket::StartThrust()
{
	m_isThrusting = true;
}

void Rocket::EndThrust()
{
	m_isThrusting = false;
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

	target.draw(triangle, states);
}



Step::Step()
{
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

void Step::SetEscapeVelocity(double ve)
{
	m_escapeVelocity = ve;
}

void Step::SetMassLoss(double m)
{
	m_fuelMassLoss = m;
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

const double& Step::GetEscapeVelocity() const
{
	return m_escapeVelocity;
}

const double& Step::GetMassLoss() const
{
	return m_fuelMassLoss;
}