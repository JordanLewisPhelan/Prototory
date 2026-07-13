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
#include "MachineRegistry.h"
#include "TileAccessor.h"
#include "MachineSystem.h"
#include "PlacementController.h"



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
	sf::Text m_inventoryText{ m_gameFont };
	sf::Text m_promptText{ m_gameFont };
	sf::Text m_inspectorText{ m_gameFont };

	TileMap m_tileMap;

	Player m_player;
	sf::View m_camera;

	WorldGenerator m_worldGen;		// Leftovers and general use
	ChunkManager m_chunkManager;	// Current implementation 

	ResourceRegistry m_resourceRegistry;
	MachineRegistry m_machineRegistry;

	TileAccessor m_tileAccessor;	
	MachineSystem m_machineSystem;
	PlacementController m_placementController;


	SceneActions m_pendingAction;

	void updateCamera();
	std::string getCurrentBiome();

	/// ToDo: replace with an actual placement for these - Inventory needs to 
	/// but not in Gameplay, this is sacreligious - but added for testing
	void renderInventory(sf::RenderWindow& t_window);
	/// Displays when facing and hovering over a harvestable tile
	void renderInteractionPrompt(sf::RenderWindow& t_window);
	/// Displays the Inventory or a machine we are hovering over - Debug to be removed/refined later
	void renderMachineInspector(sf::RenderWindow& t_window);
};