#include "FPS_Counter.hpp"
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\System\Clock.hpp>
#include <sstream>

FPS_Counter::FPS_Counter(float screenW)
{
	m_font = std::make_unique<sf::Font>();
	m_text = std::make_unique<sf::Text>();
	m_clock = std::make_unique<sf::Clock>();

	m_font->loadFromFile("../Assets/Arcon-Regular.otf");
	m_text->setFont(*m_font);
	m_text->setPosition(screenW - 40.0f, 0.0f);
	m_screenW = screenW;
}

FPS_Counter::~FPS_Counter()
{
}

void FPS_Counter::Update()
{
	static int updates = 0;
	++updates;

	if (m_clock->getElapsedTime().asSeconds() >= 1.0f)
	{
		std::ostringstream convert;
		convert << updates;

		m_text->setPosition(m_screenW - ((int)log10f(updates) + 1) * 20.0f, 0.0f);
		m_text->setString(convert.str());

		updates = 0;
		m_clock->restart();
	}
}

void FPS_Counter::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_text, states);
}