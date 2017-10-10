#ifndef INPUT_HPP
#define INPUT_HPP
#include "Vector2.hpp"
#include "Singleton.hpp"
#include <SFML\Graphics\RenderWindow.hpp>

//namespace sf
//{
//	class RenderWindow;
//}

class Input final : public Singleton<Input>
{
	friend class Singleton<Input>;

	Input();
	~Input();

public:
	enum MouseKey
	{
		LEFT, RIGHT, SCROLL_WHEEL
	};

	void UpdateInput();
	void UpdateWindow();

	void ZoomInOnMouse();

	void SetWindow(sf::RenderWindow* window);
	void SetView(sf::View* view);
	void SetEvent(sf::Event* event);

	bool IsKeyDown(unsigned int key) const;
	bool IsKeyPressed(unsigned int key) const;

	bool IsMouseDown(const MouseKey& mouseKey) const;
	bool IsMousePressed(const MouseKey& mouseKey) const;
	bool IsMouseDown(unsigned int mouseKey) const;
	bool IsMousePressed(unsigned int mouseKey) const;

	const sf::Vector2f& MousePosition() const;

	const Vector2d& MousePositionDouble() const;
	const sf::Vector2f MouseMovement() const;

private:
	bool m_keysDown[256];
	bool m_keysDownPrev[256];

	sf::Vector2f m_mousePos;
	sf::Vector2f m_mousePosPrev;

	sf::Vector2i m_mouseWindowCoord; //needed?


	bool m_mouseDown[3];
	bool m_mouseDownPrev[3];

	sf::RenderWindow* m_window;
	sf::View* m_view;
	sf::Event* m_event;
};

#endif