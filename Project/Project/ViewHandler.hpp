#ifndef VIEWHANDLER_HPP
#define VIEWHANDLER_HPP

#include "Singleton.hpp"
#include <SFML\System\Vector2.hpp>
#include "Vector2.hpp"

namespace sf
{
	class RenderWindow;
	class View;
	class Event;
}

class ViewHandler final : public Singleton<ViewHandler>
{
	friend class Singleton<ViewHandler>;

	ViewHandler();
	~ViewHandler();

public:
	void UpdateWindow();

	void ZoomInOnMouse();

	void SetWindow(sf::RenderWindow* window);
	void SetView(sf::View* view);
	void SetEvent(sf::Event* event);

	const sf::Vector2f& GetMouseWindowPixelPosition() const;
	const sf::Vector2f& GetViewSize() const;

	const Vector2d& GetOriginOffset() const;
	void UpdateOriginOffset(const sf::Vector2f& delta);
private:
	Vector2d m_origin_offset;

	sf::RenderWindow* m_window;
	sf::View* m_view;
	sf::Event* m_event;
};

#endif // !VIEWHANDLER_HPP
