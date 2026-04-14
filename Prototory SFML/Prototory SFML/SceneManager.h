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
	SceneManager(sf::RenderWindow& t_window);
	~SceneManager() = default;

	void ChangeScene(SceneType t_type);

	void HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window);
	void Update(sf::Time t_dt);
	void Render(sf::RenderWindow& t_window);
	void HandleSceneAction(SceneActions t_action);

	
	
	bool isQuitRequested();

private:
	sf::RenderWindow& m_window;
	std::unique_ptr<Scene> m_currentScene;
	bool m_isQuitRequested = false;

	void CreateScene(SceneType t_type);
};
