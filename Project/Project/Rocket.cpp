#include "Rocket.hpp"

Rocket::Rocket()
{
}

Rocket::~Rocket()
{
}

void Rocket::Update(float dt)
{
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