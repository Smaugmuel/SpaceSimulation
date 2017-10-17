#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include "Vector2.hpp"
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\Color.hpp>

namespace sf
{
	class RenderTarget;
	class RenderStates;
}

class Projectile : public sf::Drawable
{
public:
	Projectile();
	Projectile(const Vector2d& p, const Vector2d& v);
	Projectile(double x, double y, double vx, double vy);
	virtual ~Projectile();

	virtual void Update(float dt);

	void SetPosition(const Vector2d& position);
	void SetPosition(double x, double y);
	void SetVelocity(const Vector2d& velocity);
	void SetVelocity(double x, double y);
	void SetAcceleration(const Vector2d& acceleration);
	void SetAcceleration(double x, double y);
	void SetColor(const sf::Color& color);
	void SetMass(double mass);
	void SetRadius(double radius);

	void SetIsCrashed(bool isCrashed);
	
	void UpdateVisualRadius();
	void SetSpin(float spin);

	void AddPosition(const Vector2d& v);
	void AddPosition(double x, double y);
	void AddVelocity(const Vector2d& v);
	void AddVelocity(double x, double y);
	void AddAcceleration(const Vector2d& v);
	void AddAcceleration(double x, double y);


	const Vector2d& GetPosition() const;
	virtual Vector2d GetVelocity() const;
	const Vector2d& GetAcceleration() const;
	double GetMass() const;
	double GetRadius() const;
	double GetArea() const;
	float GetSpin() const;

	bool GetIsCrashed() const;

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

protected:
	Vector2d m_position;
	Vector2d m_velocity;
	Vector2d m_acceleration;

	sf::CircleShape m_circle;
	double m_mass;
	double m_visual_radius;
	double m_radius;
	double m_spin;

	bool m_isCrashed;
};

#endif