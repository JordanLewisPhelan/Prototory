#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <iostream>


class Button
{
public:
	Button(const sf::Vector2f& t_pos, const sf::Vector2f& t_size,
			const std::string& t_text, sf::Font& t_font );

	void HandleEvent(const sf::Event& t_event, const sf::Vector2f& t_mousePos);
	void Render(sf::RenderWindow& t_window);

	// Allows us to set what a button does when clicked
	void setCallback(std::function<void()> t_functionCalled);

	// Mild styling setters
	void setIdleColour(const sf::Color& t_colour);
	void setHoverColour(const sf::Color& t_colour);
	void setTextColour(const sf::Color& t_colour);


private:
	sf::RectangleShape m_buttonShape;
	sf::Text m_buttonText;

	sf::Color m_idleColour, m_hoverColour;
	bool m_isHovered;

	// We will set a function to this that will run when the button is pressed
	std::function<void()> m_functionCalled = nullptr;

	bool MouseOver(const sf::Vector2f& t_mousePos) const;
};

