#include "SceneManager.h"
#include "MainMenu.h"
#include "Gameplay.h"

SceneManager::SceneManager()
{
	/*-- Start in main menu --*/
	ChangeScene(SceneType::MainMenu);
}


void SceneManager::ChangeScene(SceneType t_type)
{
	CreateScene(t_type);
}


void SceneManager::HandleEvent(const std::optional<sf::Event>& t_event)
{

}


void SceneManager::Update(sf::Time t_dt)
{
}


void SceneManager::Render(sf::RenderWindow& t_window)
{
}



void SceneManager::CreateScene(SceneType t_type)
{
	switch (t_type)
	{
	case SceneType::MainMenu:
		m_currentScene = std::make_unique<MainMenu>();
		break;
	case SceneType::Gameplay:
		m_currentScene = std::make_unique<Gameplay>();
		break;
	}
}
