#pragma once
#include "Scene.h"
#include <iostream>
#include "Globals.h"
#include "Button.h"

class MainMenu : public Scene
{
public:
	MainMenu();
	~MainMenu() override = default;

	void HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window) override;
	void Update(sf::Time t_dt) override;
	void Render(sf::RenderWindow& t_window) override;

	SceneActions getRequestedAction() override;

private:
	sf::Font m_menuFont;
	sf::Text m_titleText{m_menuFont};

	
	std::vector<Button> m_sceneButtons;

	SceneActions m_pendingAction;
	
	void GenerateButtons();
};

