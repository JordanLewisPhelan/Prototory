#pragma once
#include "Scene.h"
#include <memory>
#include <vector>


// Houses types of Scenes : Note: Options will be an overlay not a scene directly
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

	void HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window);
	void Update(sf::Time t_dt);
	void Render(sf::RenderWindow& t_window);
	void HandleSceneAction(SceneActions t_action);

	// Adding to effectively fully remove Game class outside of setting up game content
	// i want to delegate with these new scene classes as it is more controllable and under 1 banner
	bool isQuitRequested();

private:
	std::unique_ptr<Scene> m_currentScene;
	bool m_isQuitRequested = false;

	void CreateScene(SceneType t_type);
};
