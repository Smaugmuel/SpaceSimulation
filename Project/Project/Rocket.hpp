#ifndef ROCKET_HPP
#define ROCKET_HPP
#include <list>
#include "Vector2.hpp"
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\CircleShape.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class Planet;

class Stage
{
public:
	Stage();
	~Stage();

	void Update(float dt);

	void SetHullMass(double mass);
	void SetFuelMass(double mass);
	void SetExhaustVelocity(double ve);
	void SetBurnRate(double m);

	void AddFuelMass(double mass);

	const double& GetHullMass() const;
	const double& GetFuelMass() const;
	const double& GetTotalMass() const;
	const double& GetExhaustVelocity() const;
	const double& GetBurnRate() const;
	double GetThrust() const;

private:
	double m_exhaustVelocity;
	double m_hullMass;
	double m_fuelMass;
	double m_burnRate;
};

class Rocket : public sf::Drawable
{
public:
	Rocket(Vector2d position, Vector2d velocity, Vector2d acceleration, Vector2d direction, Planet* startPlanet);
	Rocket(double x, double y, double vx, double vy, double ax, double ay, double dx, double dy, Planet* startPlanet);
	~Rocket();

	void Update(float dt);
	void UpdateThrust(float dt);
	void UpdateRotation();

	void SetPosition(const Vector2d& position);
	void SetPosition(double x, double y);
	void SetVelocity(const Vector2d& velocity);
	void SetVelocity(double x, double y);
	void SetAcceleration(const Vector2d& acceleration);
	void SetAcceleration(double x, double y);
	void SetRotation(double rotation);
	void SetPayloadMass(double mass);
	void SetOrbitedPlanet(Planet* planet);
	
	void ToggleThrust();

	void AddPosition(const Vector2d& v);
	void AddPosition(double x, double y);
	void AddVelocity(const Vector2d& v);
	void AddVelocity(double x, double y);
	void AddAcceleration(const Vector2d& v);
	void AddAcceleration(double x, double y);

	void AddStage(double grossMass, double emptyMass, double specificImpulse, double burnTime);

	const Vector2d& GetPosition() const;
	const Vector2d& GetVelocity() const;
	const Vector2d& GetAcceleration() const;
	const double& GetPayloadMass() const;
	const double& GetTotalMass() const;

	const bool& GetIsThrusting() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::CircleShape m_triangle;

	Planet* m_orbitedPlanet;

	Vector2d m_position;
	Vector2d m_velocity;
	Vector2d m_acceleration;
	Vector2d m_direction;		// Direction of triangle
	Vector2d m_startDirection;

	double m_payloadMass;

	std::list<Stage*> m_stages;

	bool m_isThrusting;


	int m_flipPreventionCounter;
};

#endif