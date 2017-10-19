#include "Rocket.hpp"
#include "SystemInformation.hpp"
#include "ViewHandler.hpp"
#include "Planet.hpp"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Font.hpp>
#include <sstream>
#include <iomanip>
#include <locale>

struct num_grouping: std::numpunct<char> {
	char do_thousands_sep() const { return ','; }
	std::string do_grouping() const { return "\03"; }
};

Rocket::Rocket(Vector2d position, Vector2d velocity, Vector2d acceleration, Vector2d direction, Planet* startPlanet)
	: Rocket::Rocket(position.x, position.y, velocity.x, velocity.y, acceleration.x, acceleration.y, direction.x, direction.y, startPlanet)
{
}

Rocket::Rocket(double x, double y, double vx, double vy, double ax, double ay, double dx, double dy, Planet* startPlanet)
{
	m_position = Vector2d(x, y);
	m_velocity = Vector2d(vx, vy);
	m_acceleration = Vector2d(ax, ay);
	m_direction = m_startDirection = Vector2d(dx, dy);

	// Data from Saturn V
	m_payloadMass = 48600;
	AddStage(2290000, 130000, 263, 165);
	AddStage(496200, 40100, 421, 360);
	AddStage(123000, 13500, 421, 500);

	m_orbitedPlanet = startPlanet;

	m_isThrusting = true;
	m_reachedLEO = false;
	m_reachedEscapeVelocity = false;

	m_flipPreventionCounter = 0;

	m_triangle.setPointCount(3);
	m_triangle.setRadius(2.0f);
	m_triangle.setOrigin(2.0f, 2.0f);
	m_triangle.setPosition(x * PX_PER_M, y * PX_PER_M);
	m_triangle.setScale(1.0f, 2.0f);

	UpdateColor();

	m_isCrasched = false;

	int positive = m_direction.y / std::abs(m_direction.y);
	double rotation = positive * std::acosf(m_direction.x) * 180 / 3.1415927f;
	m_triangle.setRotation(90 - rotation);

	m_font = std::make_unique<sf::Font>();
	m_text = std::make_unique<sf::Text>();

	m_font->loadFromFile("../Assets/cour.ttf");
	m_text->setFont(*m_font);
	m_text->setColor(sf::Color::White);
	UpdateText();
	m_text->setOrigin(m_text->getLocalBounds().width / 2.0f, -m_text->getLocalBounds().height / 2.0f);
	m_timeCrashed = 0;
	m_explodingRadius = 0;
}

Rocket::~Rocket()
{
	while (m_stages.size())
	{
		delete m_stages.front();
		m_stages.pop_front();
	}
}

void Rocket::Update(float dt)
{
	m_position += m_velocity * dt;
	m_velocity += m_acceleration * dt;

	if (!m_reachedLEO)
	{
		double distance = (m_position - m_orbitedPlanet->GetPosition()).Length();
		double aboveSurface = distance - m_orbitedPlanet->GetRadius();
		if (aboveSurface > 170000)
		{
			m_reachedLEO = true;
			m_triangle.setFillColor(sf::Color::Blue);


			// Turn sideways when reaching low earth orbit
			Vector2d planetToRocket = (m_position - m_orbitedPlanet->GetPosition()).Normalized();
			m_direction = (planetToRocket.Orthogonal() + planetToRocket * 0.3).Normalized();
		}
	}

	if (!m_reachedEscapeVelocity)
	{
		if (AchievedEscapeVelocityAroundOrbitedPlanet())
		{
			m_reachedEscapeVelocity = true;
			m_triangle.setFillColor(sf::Color::Magenta);
		}
	}
	UpdateText();
}

void Rocket::UpdateThrust(float dt)
{
	if (m_stages.empty())
	{
		m_isThrusting = false;
	}

	if (m_isThrusting)
	{
		Stage* stage = m_stages.front();

		// Mass of fuel used during this iteration
		double burnAmount = stage->GetBurnRate() * dt;

		// Acceleration based on mass before burning fuel
		double acceleration = stage->GetThrust() / GetTotalMass();

		// Decrease fuel. Remove stage if fuel is out
		stage->AddFuelMass(-burnAmount);
		if (stage->GetFuelMass() < 0.0)
		{
			delete stage;
			m_stages.pop_front();

			UpdateColor();
		}

		m_acceleration += m_direction * acceleration;
	}
}

void Rocket::UpdateRotation()
{
	if (m_reachedLEO && !m_stages.empty() && !AchievedEscapeVelocityAroundOrbitedPlanet())
	{
		Vector2d planetToRocket = m_position - m_orbitedPlanet->GetPosition();
		m_direction = (planetToRocket.Orthogonal() + planetToRocket * 0.2).Normalized();
	}

	int positive = m_direction.y / std::abs(m_direction.y);
	double rotation = positive * std::acosf(m_direction.x) * 180 / 3.1415927f;
	
	m_triangle.setRotation(90 - rotation);
}

void Rocket::UpdateText()
{
	double velocity = (m_velocity - m_orbitedPlanet->GetVelocity()).Length();
	double distanceToEarth = ((m_position - m_orbitedPlanet->GetPosition()).Length() - m_orbitedPlanet->GetRadius()) * 0.001;

	std::ostringstream ss;
	ss.imbue(std::locale(ss.getloc(), new num_grouping));
	ss << "v_e: " << std::right << std::setfill(' ') << std::setw(9) << std::setprecision(0) << std::fixed << velocity << "m/s \n";
	ss << "v_s: " << std::right << std::setfill(' ') << std::setw(9) << std::setprecision(0) << std::fixed << m_velocity.Length() << "m/s \n";
	ss << "d_e: " << std::right << std::setfill(' ') << std::setw(9) << std::setprecision(0) << std::fixed << distanceToEarth << "km";

	this->m_text->setString(ss.str());
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
	m_direction = Vector2d(std::cos(rotation), std::sin(rotation));
}

void Rocket::SetPayloadMass(double mass)
{
	m_payloadMass = mass;
}

void Rocket::SetOrbitedPlanet(Planet * planet)
{
	m_orbitedPlanet = planet;
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

void Rocket::AddStage(double grossMass, double emptyMass, double specificImpulse, double burnTime)
{
	Stage* stage = new Stage;

	stage->SetFuelMass(grossMass - emptyMass);
	stage->SetHullMass(emptyMass);
	stage->SetExhaustVelocity(specificImpulse * 9.82);
	stage->SetBurnRate(stage->GetFuelMass() / burnTime);

	m_stages.push_back(stage);
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

	for (auto& stage : m_stages)
	{
		mass += stage->GetTotalMass();
	}
	return mass;
}

const bool & Rocket::GetIsThrusting() const
{
	return m_isThrusting;
}

Planet * Rocket::GetOrbitedPlanet() const
{
	return m_orbitedPlanet;
}

bool Rocket::AchievedEscapeVelocityAroundOrbitedPlanet() const
{
	Vector2d v_rel = m_velocity - m_orbitedPlanet->GetVelocity();
	Vector2d distance = m_position - m_orbitedPlanet->GetPosition();

	double escapeVelocity = std::sqrt(6.67408e-11 * m_orbitedPlanet->GetMass() / distance.Length());

	return v_rel.Length() > escapeVelocity;
}

void Rocket::UpdateColor()
{
	switch (m_stages.size())
	{
	case 0:
		m_triangle.setFillColor(sf::Color::Red);
		break;
	case 1:
		m_triangle.setFillColor(sf::Color(255, 140, 0));	// Orange
		break;
	case 2:
		m_triangle.setFillColor(sf::Color::Yellow);
		break;
	case 3:
		m_triangle.setFillColor(sf::Color::Green);
		break;
	default:
		break;
	}
}

bool Rocket::GetCrashed()const {
	return m_isCrasched;
}

void Rocket::SetIsCrashed(bool isCrashed) {
	m_isCrasched = isCrashed;
	m_explodingRadius = 1e+6;
	m_triangle.setPointCount(150);

	if (m_isCrasched)
	{
		m_isThrusting = false;
	}
}

void Rocket::SetColor(sf::Color color) {
	m_triangle.setFillColor(color);
}

void Rocket::SetRadius(double radius) {
	m_explodingRadius = radius;
	m_triangle.setRadius(m_explodingRadius);
}

double Rocket::GetRadius()const {
	return m_explodingRadius;
}

void Rocket::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	sf::CircleShape triangle = m_triangle;

	Vector2f screenPos = (m_position - ViewHandler::Get()->GetOriginOffset()) * PX_PER_M;
	triangle.setPosition(screenPos.x, WNDH - screenPos.y);
	triangle.setScale(sf::Vector2f(1.0f, 2.0f) * (ViewHandler::Get()->GetViewSize().x * 0.002f));

	target.draw(triangle, states);

	m_text->setScale(ViewHandler::Get()->GetViewSize() * 0.0004f);
	m_text->setPosition(screenPos.x, WNDH - screenPos.y);

	target.draw(*m_text, states);
}





Stage::Stage()
{
	/*
	First stage
	-----------------------------------

	Total mass 
	Gross mass of first stage	m = 2,290,000 kg
	Thrust at sea level			35,100,000 N
	Specific impulse			263 s
	Burn time					165 s

	
	*/

	// Gross mass: 2290000

	// Approximate values taken from Saturn V's first stage
	m_hullMass = 100000;
	m_fuelMass = 2000000;
	m_burnRate = 1000000;
	m_exhaustVelocity = 2000;
}

Stage::~Stage()
{
}

void Stage::Update(float dt)
{
}

void Stage::SetHullMass(double mass)
{
	m_hullMass = mass;
}

void Stage::SetFuelMass(double mass)
{
	m_fuelMass = mass;
}

void Stage::SetExhaustVelocity(double ve)
{
	m_exhaustVelocity = ve;
}

void Stage::SetBurnRate(double m)
{
	m_burnRate = m;
}

void Stage::AddFuelMass(double mass)
{
	m_fuelMass += mass;
}

const double& Stage::GetHullMass() const
{
	return m_hullMass;
}

const double& Stage::GetFuelMass() const
{
	return m_fuelMass;
}

const double& Stage::GetTotalMass() const
{
	return m_hullMass + m_fuelMass;
}

const double& Stage::GetExhaustVelocity() const
{
	return m_exhaustVelocity;
}

const double& Stage::GetBurnRate() const
{
	return m_burnRate;
}

double Stage::GetThrust() const
{
	return m_burnRate * m_exhaustVelocity;
}