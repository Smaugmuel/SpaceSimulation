#include "OrbitingPlanet.hpp"
#include "Projectile.hpp"

OrbitingPlanet::OrbitingPlanet(Planet* orbitedPlanet) :
	Planet(),
	m_orbitedPlanet(orbitedPlanet),
	m_orbitAngle(0.0),
	m_orbitRadius(200.0)
{
}

OrbitingPlanet::~OrbitingPlanet()
{
}

void OrbitingPlanet::Update(float dt)
{
	m_orbitAngle += 2 * 3.1415927 / m_orbitTime * dt;

	// Set position based on angle
	Vector2d difference = Vector2d(cosf(m_orbitAngle), sinf(m_orbitAngle)) * m_orbitRadius;
	SetPosition(m_orbitedPlanet->GetPosition() + difference);

	// Update orbiting planets
	Planet::Update(dt);
}

void OrbitingPlanet::SetOrbitAngle(double startAngle)
{
	m_orbitAngle = startAngle;
}

void OrbitingPlanet::SetOrbitRadius(double radius)
{
	m_orbitRadius = radius;
}

void OrbitingPlanet::SetOrbitTime(double time)
{
	m_orbitTime = time;
}
