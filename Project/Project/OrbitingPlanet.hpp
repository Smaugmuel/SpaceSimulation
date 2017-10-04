#ifndef ORBITING_PLANET_HPP
#define ORBITING_PLANET_HPP
#include "Planet.hpp"

class OrbitingPlanet : public Planet
{
public:
	OrbitingPlanet(Planet* orbitedPlanet);
	~OrbitingPlanet();

	void Update(float dt) override;

	void SetOrbitAngle(double startAngle);
	void SetOrbitRadius(double radius);
	void SetOrbitTime(double time);

private:
	Planet* m_orbitedPlanet;
	double m_orbitRadius;
	double m_orbitAngle;
	double m_orbitTime;
};

#endif