#pragma once
#include "Scene.h"
#include <iostream>
#include "Globals.h"
#include "Player.h"
#include "TileMap.h"


class Gameplay : public Scene
{
public:
	Gameplay();
	~Gameplay() override = default;

	void HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window) override;
	void Update(sf::Time t_dt) override;
	void Render(sf::RenderWindow& t_window) override;

	SceneActions getRequestedAction() override;

private:
	sf::Font m_gameFont;
	sf::Text m_areaText{m_gameFont};

	TileMap m_tileMap;

	Player m_player;
	sf::View m_camera;

	SceneActions m_pendingAction;

	void updateCamera();
};