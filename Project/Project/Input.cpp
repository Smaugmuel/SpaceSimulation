#include "Input.hpp"
#include "ViewHandler.hpp"
#include <SFML\Window\Window.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Window\Keyboard.hpp>
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
	m_mousePos = ViewHandler::Get()->GetMouseWindowPixelPosition();

	m_mousePosDouble = Vector2d(m_mousePos.x, WNDH - m_mousePos.y);

	m_mouseDownPrev[0] = m_mouseDown[0];
	m_mouseDownPrev[1] = m_mouseDown[1];
	m_mouseDownPrev[2] = m_mouseDown[2];

	m_mouseDown[0] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	m_mouseDown[1] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
	m_mouseDown[2] = sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle);
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