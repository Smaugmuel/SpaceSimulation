#include "Input.hpp"
#include <SFML\Window\Window.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include "SystemInformation.hpp"

//for debugging
#include <iostream>

Input* Singleton<Input>::s_instance = nullptr;

Input::Input()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_keysDown[i] = false;
		m_keysDownPrev[i] = false;
	}

	for (unsigned int i = 0; i < 2; i++)
	{
		m_mouseDown[i] = false;
		m_mouseDownPrev[i] = false;
	}
}

Input::~Input()
{
}

void Input::UpdateInput()
{
	for (unsigned int i = 0; i < 256; i++)
	{
		m_keysDownPrev[i] = m_keysDown[i];
		m_keysDown[i] = sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i));
	}

	m_mousePosPrev = m_mousePos;
	m_mouseWindowCoord = sf::Mouse::getPosition(*m_window);
	m_mousePos = m_window->mapPixelToCoords(m_mouseWindowCoord);

	m_mousePosDouble = Vector2d(m_mousePos.x, WNDH - m_mousePos.y);

	m_mouseDownPrev[0] = m_mouseDown[0];
	m_mouseDownPrev[1] = m_mouseDown[1];
	m_mouseDownPrev[2] = m_mouseDown[2];

	m_mouseDown[0] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	m_mouseDown[1] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	m_mouseDown[2] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
}

void Input::UpdateWindow()
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

					m_view->setCenter(m_view->getCenter() + delta);
					m_window->setView(*m_view);
				}
				oldPos = m_window->mapPixelToCoords(sf::Vector2i(m_event->mouseMove.x, m_event->mouseMove.y));
			}
			break;
		default:
			break;
		}
	}
}

void Input::ZoomInOnMouse()
{
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
	const sf::Vector2f beforeCoord{ m_window->mapPixelToCoords(pixelPos) };
	m_view->zoom(1.0f - m_event->mouseWheel.delta * 0.1f);
	m_window->setView(*m_view);

	const sf::Vector2f afterCoord{ m_window->mapPixelToCoords(pixelPos) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	m_view->move(offsetCoords);
	m_window->setView(*m_view);
}


void Input::SetWindow(sf::RenderWindow * window)
{
	m_window = window;
}

void Input::SetView(sf::View* view)
{
	m_view = view;
}

void Input::SetEvent(sf::Event* event)
{
	m_event = event;
}
bool Input::IsKeyDown(unsigned int key) const
{
	return m_keysDown[key];
}

bool Input::IsKeyPressed(unsigned int key) const
{
	return m_keysDown[key] && !m_keysDownPrev[key];
}

bool Input::IsMouseDown(const MouseKey & mouseKey) const
{
	return m_mouseDown[mouseKey];
}

bool Input::IsMousePressed(const MouseKey & mouseKey) const
{
	return m_mouseDown[mouseKey] && !m_mouseDownPrev[mouseKey];
}

bool Input::IsMouseDown(unsigned int mouseKey) const
{
	return m_mouseDown[mouseKey];
}

bool Input::IsMousePressed(unsigned int mouseKey) const
{
	return m_mouseDown[mouseKey] && !m_mouseDownPrev[mouseKey];
}

const sf::Vector2f& Input::MousePosition() const
{
	return m_mousePos;
}

const Vector2d& Input::MousePositionDouble() const
{
	return m_mousePosDouble;
}

const sf::Vector2f Input::MouseMovement() const
{
	return m_mousePos - m_mousePosPrev;
}