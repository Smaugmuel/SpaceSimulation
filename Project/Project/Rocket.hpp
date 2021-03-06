#ifndef ROCKET_HPP
#define ROCKET_HPP
#include <vector>
#include "Vector2.hpp"
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\CircleShape.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class Step
{
public:
	Step();
	~Step();

	void Update(float dt);

	void SetHullMass(double mass);
	void SetFuelMass(double mass);
	void SetEscapeVelocity(double ve);
	void SetMassLoss(double m);

	const double& GetHullMass() const;
	const double& GetFuelMass() const;
	const double& GetTotalMass() const;
	const double& GetEscapeVelocity() const;
	const double& GetMassLoss() const;

private:
	double m_escapeVelocity;
	double m_hullMass;
	double m_fuelMass;
	double m_fuelMassLoss;
};

class Rocket : public sf::Drawable
{
public:
	Rocket(Vector2d position, Vector2d velocity);
	Rocket(double x, double y, double vx, double vy);
	Rocket();
	~Rocket();

	void Update(float dt);

	void SetPosition(const Vector2d& position);
	void SetPosition(double x, double y);
	void SetVelocity(const Vector2d& velocity);
	void SetVelocity(double x, double y);
	void SetAcceleration(const Vector2d& acceleration);
	void SetAcceleration(double x, double y);

	void SetPayloadMass(double mass);
	
	void StartThrust();
	void EndThrust();

	void AddPosition(const Vector2d& v);
	void AddPosition(double x, double y);
	void AddVelocity(const Vector2d& v);
	void AddVelocity(double x, double y);
	void AddAcceleration(const Vector2d& v);
	void AddAcceleration(double x, double y);

	const Vector2d& GetPosition() const;
	const Vector2d& GetVelocity() const;
	const Vector2d& GetAcceleration() const;
	const double& GetPayloadMass() const;
	const double& GetTotalMass() const;

	const bool& GetIsThrusting() const;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::CircleShape m_triangle;

	Vector2d m_position;
	Vector2d m_velocity;
	Vector2d m_acceleration;

	double m_payloadMass;

	std::vector<Step*> m_steps;

	bool m_isThrusting;
};

#endif