#pragma once
#include "Scene.h"
#include <iostream>
#include "Globals.h"
#include "Player.h"
#include "TileMap.h"
#include "WorldGenerator.h"
#include "ChunkManager.h"
#include "LoadingScreen.h"
#include "ResourceRegistry.h"


class Gameplay : public Scene
{
public:
	Gameplay(sf::RenderWindow& t_window);
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

	WorldGenerator m_worldGen;		// Leftovers and general use
	ChunkManager m_chunkManager;	// Current implementation 

	ResourceRegistry m_resourceRegistry;

	SceneActions m_pendingAction;

	void updateCamera();
	std::string getCurrentBiome();
};