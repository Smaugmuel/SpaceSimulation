#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP
#include <SFML\Graphics\Drawable.hpp>
#include <memory>

namespace sf
{
	class Font;
	class Text;
	class Clock;
}

class FPS_Counter : public sf::Drawable
{
public:
	FPS_Counter(float screenW);
	~FPS_Counter();

	void Update();

private:
	std::unique_ptr<sf::Font> m_font;
	std::unique_ptr<sf::Text> m_text;
	std::unique_ptr<sf::Clock> m_clock;
	float m_screenW;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

#endif