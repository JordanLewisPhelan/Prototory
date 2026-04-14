#include "Button.h"

Button::Button(const sf::Vector2f& t_pos, const sf::Vector2f& t_size,
	const std::string& t_text, sf::Font& t_font)
	: m_idleColour(sf::Color::Red), m_hoverColour(sf::Color::Green),
	m_isHovered(false), m_buttonText(t_font)
{
	
	m_buttonShape.setPosition(t_pos);
	m_buttonShape.setSize(t_size);
	m_buttonShape.setFillColor(m_idleColour);

	
	m_buttonText.setCharacterSize(20);
	m_buttonText.setString(t_text);
	m_buttonText.setFillColor(sf::Color::Black);

	
	sf::FloatRect textBounds = m_buttonText.getLocalBounds();
	m_buttonText.setOrigin(sf::Vector2f(
		textBounds.size.x / 2.f, textBounds.size.y / 2.f
	));
	m_buttonText.setPosition(sf::Vector2f(
		t_pos.x + t_size.x / 2.f,
		t_pos.y + t_size.y / 2.f
	));
}

void Button::HandleEvent(const sf::Event& t_event, const sf::Vector2f& t_mousePos)
{
	
	if (t_event.is<sf::Event::MouseMoved>())
	{
		m_isHovered = MouseOver(t_mousePos);
		m_buttonShape.setFillColor(m_isHovered ? m_hoverColour : m_idleColour);
	}

	
	if (t_event.is<sf::Event::MouseButtonReleased>())
	{
		if (m_isHovered && m_functionCalled)
		{
			m_functionCalled();
		}
	}
}

void Button::Render(sf::RenderWindow& t_window)
{
	t_window.draw(m_buttonShape);
	t_window.draw(m_buttonText);
}

void Button::setCallback(std::function<void()> t_functionCalled)
{
	m_functionCalled = t_functionCalled;
}

void Button::setIdleColour(const sf::Color& t_colour)
{
	m_idleColour = t_colour;

	
	if (!m_isHovered)
	{
		m_buttonShape.setFillColor(m_idleColour);
	}
}

void Button::setHoverColour(const sf::Color& t_colour)
{
	m_hoverColour = t_colour;

	
	if (m_isHovered) 
	{
		m_buttonShape.setFillColor(m_hoverColour);
	}
}

void Button::setTextColour(const sf::Color& t_colour)
{
	m_buttonText.setFillColor(t_colour);
}

bool Button::MouseOver(const sf::Vector2f& t_mousePos) const
{
	return m_buttonShape.getGlobalBounds().contains(t_mousePos);
}
