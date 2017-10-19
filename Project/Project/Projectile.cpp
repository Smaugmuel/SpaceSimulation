#include "Projectile.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include "SystemInformation.hpp"
#include "ViewHandler.hpp"

Projectile::Projectile()
	: Projectile::Projectile(0.0f, 0.0f, 0.0f, 0.0f)
{
}

Projectile::Projectile(const Vector2d & p, const Vector2d & v)
	: Projectile::Projectile(p.x, p.y, v.x, v.y)
{
}

Projectile::Projectile(double x, double y, double vx, double vy)
{
	m_position = Vector2d(x, y);
	m_velocity = Vector2d(vx, vy);
	m_acceleration = Vector2d(0, 0);

	m_mass = 100;
	m_spin = 0.0f;
	m_radius = 1e+6;

	SetRadius(1e+6);

	m_circle.setFillColor(sf::Color::Red);
	m_circle.setPosition(x * PX_PER_M, y * PX_PER_M);
	m_circle.setPointCount(200);

	m_isCrashed = false;
}

Projectile::~Projectile()
{
}

void Projectile::Update(float dt)
{
	m_position += m_velocity * dt;
	m_velocity += m_acceleration * dt;
}

void Projectile::SetPosition(const Vector2d & position)
{
	m_position = position;
}

void Projectile::SetPosition(double x, double y)
{
	m_position.x = x;
	m_position.y = y;
}

void Projectile::SetVelocity(const Vector2d & velocity)
{
	m_velocity = velocity;
}

void Projectile::SetVelocity(double x, double y)
{
	m_velocity.x = x;
	m_velocity.y = y;
}

void Projectile::SetAcceleration(const Vector2d & acceleration)
{
	m_acceleration = acceleration;
}

void Projectile::SetAcceleration(double x, double y)
{
	m_acceleration.x = x;
	m_acceleration.y = y;
}

void Projectile::SetColor(const sf::Color & color)
{
	m_circle.setFillColor(color);
}

void Projectile::SetMass(double mass)
{
	m_mass = mass;
}

void Projectile::SetRadius(double radius)
{
	m_radius = radius;
	UpdateVisualRadius();
}

void Projectile::SetIsCrashed(bool isCrashed)
{
	m_isCrashed = isCrashed;
}

void Projectile::UpdateVisualRadius()
{
	m_visual_radius = m_radius * PX_PER_M;//std::powf(2, std::log10f(m_radius) - 5.5);
	
	// Actual sizes.
	//m_visual_radius = m_radius * PX_PER_M;

	m_circle.setRadius(m_visual_radius);
	m_circle.setOrigin(m_visual_radius, m_visual_radius);
}

void Projectile::SetSpin(float spin)
{
	m_spin = spin;
}

void Projectile::AddPosition(const Vector2d & v)
{
	m_position += v;
}

void Projectile::AddPosition(double x, double y)
{
	m_position.x += x;
	m_position.y += y;
}

void Projectile::AddVelocity(const Vector2d & v)
{
	m_velocity += v;
}

void Projectile::AddVelocity(double x, double y)
{
	m_velocity.x += x;
	m_velocity.y += y;
}

void Projectile::AddAcceleration(const Vector2d & v)
{
	m_acceleration += v;
}

void Projectile::AddAcceleration(double x, double y)
{
	m_acceleration.x += x;
	m_acceleration.y += y;
}

const Vector2d & Projectile::GetPosition() const
{
	return m_position;
}

Vector2d Projectile::GetVelocity() const
{
	return m_velocity;
}

const Vector2d & Projectile::GetAcceleration() const
{
	return m_acceleration;
}

double Projectile::GetRadius() const
{
	return m_radius;
}

double Projectile::GetArea() const
{
	return m_radius * m_radius * 3.1415927;
}

double Projectile::GetMass() const
{
	return m_mass;
}

float Projectile::GetSpin() const
{
	return m_spin;
}

bool Projectile::GetIsCrashed()const
{
	return m_isCrashed;
}

void Projectile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::CircleShape circle = m_circle;

	Vector2d screenPos = (m_position - ViewHandler::Get()->GetOriginOffset())* PX_PER_M;

	circle.setPosition(screenPos.x, WNDH - screenPos.y);
	circle.setScale(sf::Vector2f(1.0f, 1.0f) * (ViewHandler::Get()->GetViewSize().x * 0.002f));

	target.draw(circle, states);
}