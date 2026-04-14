#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <optional>
#include <functional>

enum class SceneActions
{
	None,
	SwitchToGameplay,
	SwitchToMenu,
	QuitGame
};

class Scene
{
public:
	virtual ~Scene() = default;

	
	virtual void HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window) = 0;
	virtual void Update(sf::Time t_deltaTime) = 0;
	virtual void Render(sf::RenderWindow& t_window) = 0;

	virtual SceneActions getRequestedAction() = 0;
};