#include "SceneManager.h"
#include "MainMenu.h"
#include "Gameplay.h"

SceneManager::SceneManager(sf::RenderWindow& t_window)
	: m_window(t_window)
{
	/*-- Start in main menu --*/
	ChangeScene(SceneType::MainMenu);
}


void SceneManager::ChangeScene(SceneType t_type)
{
	CreateScene(t_type);
}


void SceneManager::CreateScene(SceneType t_type)
{
	switch (t_type)
	{
	case SceneType::MainMenu:
		m_currentScene = std::make_unique<MainMenu>();
		break;
	case SceneType::Gameplay:
		m_currentScene = std::make_unique<Gameplay>(m_window);
		break;
	}
}


/* --
Goal for these 3 functions is to let the active Scene(if any) determine and recieve info
We do not need to concern ourselves with anything happening on an inactive Scene(which will rarely
happen but future proofing this will allow for that safety of separating Game Inputs and other Scene
Inputs : Current Idea is to have Console Commands in game Main Menu can have a debug environment seed command
and in game this won't work but you can give items and so forth. Separation & Delegation is the goal
SceneManager does not care about the scene, it just manages.
-- */
void SceneManager::HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window)
{
	if (m_currentScene)
	{
		m_currentScene->HandleEvent(t_event, t_window);
	}
}

void SceneManager::Update(sf::Time t_dt)
{
	if (m_currentScene)
	{
		m_currentScene->Update(t_dt);

		SceneActions l_action = m_currentScene->getRequestedAction();
		HandleSceneAction(l_action);
	}
}

void SceneManager::Render(sf::RenderWindow& t_window)
{
	if (m_currentScene)
	{
		m_currentScene->Render(t_window);
	}
}

void SceneManager::HandleSceneAction(SceneActions t_action)
{
	switch (t_action)
	{
	case SceneActions::SwitchToGameplay:
		CreateScene(SceneType::Gameplay);
		break;

	case SceneActions::SwitchToMenu:
		CreateScene(SceneType::MainMenu);
		break;

	case SceneActions::QuitGame:
		m_isQuitRequested = true;
		std::cout << "Quit requested\n";
		break;

	case SceneActions::None:
	default:
		// Do nothing - scene wants to continue
		break;
	}
}

bool SceneManager::isQuitRequested()
{
	return m_isQuitRequested;
}
