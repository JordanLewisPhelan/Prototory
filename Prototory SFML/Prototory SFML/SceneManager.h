#pragma once
#include "Scene.h"
#include <memory>
#include <vector>


enum class SceneType
{
	MainMenu,
	Gameplay
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager() = default;

	void ChangeScene(SceneType t_type);

	void HandleEvent(const std::optional<sf::Event>& t_event);
	void Update(sf::Time t_dt);
	void Render(sf::RenderWindow& t_window);

private:
	std::unique_ptr<Scene> m_currentScene;

	void CreateScene(SceneType t_type);
};
