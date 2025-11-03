#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>

class Scene
{
public:
	virtual ~Scene() = default;

	// Reframing the Game Loop into scene segmentation - Will allow more specified controls in different menus or scenes
	virtual void HandleEvent(const std::optional<sf::Event>& t_event) = 0;
	virtual void Update(sf::Time t_deltaTime) = 0;
	virtual void Render(sf::RenderWindow& t_window) = 0;
};