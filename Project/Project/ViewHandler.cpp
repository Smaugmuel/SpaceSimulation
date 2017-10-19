#include "ViewHandler.hpp"
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>
#include "Input.hpp"


ViewHandler* Singleton<ViewHandler>::s_instance = nullptr;

ViewHandler::ViewHandler()
{

}

ViewHandler::~ViewHandler()
{
}

void ViewHandler::UpdateWindow()
{
	while (m_window->pollEvent(*m_event))
	{
		switch (m_event->type)
		{
		case sf::Event::Closed:
			m_window->close();
			break;
		case sf::Event::MouseWheelMoved:
			ZoomInOnMouse();
			break;
		case sf::Event::MouseMoved:
		{
			static sf::Vector2f oldPos = m_window->mapPixelToCoords(sf::Vector2i(m_event->mouseMove.x, m_event->mouseMove.y));

			if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle))
			{
				const sf::Vector2f newPos = m_window->mapPixelToCoords(sf::Vector2i(m_event->mouseMove.x, m_event->mouseMove.y));
				const sf::Vector2f delta = oldPos - newPos;

				UpdateOriginOffset(delta);
			}
			oldPos = m_window->mapPixelToCoords(sf::Vector2i(m_event->mouseMove.x, m_event->mouseMove.y));
		}
		break;
		default:
			break;
		}
	}
}

void ViewHandler::ZoomInOnMouse()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
	const sf::Vector2f beforeCoord{ m_window->mapPixelToCoords(pixelPos) };
	m_view->zoom(1.0f - m_event->mouseWheel.delta * 0.1f);
	m_window->setView(*m_view);

	const sf::Vector2f afterCoord{ m_window->mapPixelToCoords(pixelPos) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	UpdateOriginOffset(offsetCoords);
}

const sf::Vector2f& ViewHandler::GetMouseWindowPixelPosition() const
{
	return m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window));
}

const sf::Vector2f& ViewHandler::GetViewSize() const
{
	return m_view->getSize();
}


void ViewHandler::SetWindow(sf::RenderWindow * window)
{
	m_window = window;
}

void ViewHandler::SetView(sf::View* view)
{
	m_view = view;
}

void ViewHandler::SetHUDView(sf::View* view)
{
	m_hud_view = view;
}

void ViewHandler::SetEvent(sf::Event* event)
{
	m_event = event;
}

void ViewHandler::SetViewToWindow(sf::View* view)
{
	m_window->setView(*view);
}

const Vector2d& ViewHandler::GetOriginOffset() const
{
	return m_origin_offset;
}

void ViewHandler::UpdateOriginOffset(const sf::Vector2f& delta)
{
	m_origin_offset.x += delta.x;
	m_origin_offset.y -= delta.y;
}