#include "Gameplay.h"

Gameplay::Gameplay(sf::RenderWindow& t_window) 
	: m_pendingAction(SceneActions::None)
	, m_player(sf::Vector2f(64.f * 64.f, 64.f * 64.f))
	, m_tileMap(Globals::WORLD_WIDTH, Globals::WORLD_HEIGHT, Globals::TILE_SIZE)
	, m_tileAccessor(m_chunkManager)
	, m_machineSystem(m_tileAccessor, m_machineRegistry, m_resourceRegistry)
	, m_placementController(m_machineRegistry, m_tileAccessor, m_gameFont)
{
	if (!m_gameFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "Gameplay: Font has not been loaded. \n";
	}

	// Used to Show progression of loading process 
	LoadingScreen l_loadingScreen(t_window, m_gameFont);

	// World Gen with FIXED SEED for debugging
	//m_worldGen.generateSeededWorld(m_tileMap, 5878514, &l_loadingScreen);
	
	// Random World Generation - as long as a "l_seed" is used
	uint32_t l_seed = m_worldGen.generateSeed();
	m_chunkManager.initialize(l_seed, m_tileMap, &l_loadingScreen);	// debugging ; Const seed: 5878514

	// default text for debugging - will refine later(intended to show key data later)
	m_areaText.setString("Gameplay Screen");
	m_areaText.setCharacterSize(36);
	m_areaText.setFillColor(sf::Color::White);
	m_areaText.setPosition(sf::Vector2f(
		Globals::SCREEN_WIDTH / 2.5f, Globals::SCREEN_HEIGHT / 3.5f
	));
	m_areaText.setStyle(sf::Text::Underlined | sf::Text::Italic);	

	m_inventoryText.setCharacterSize(18);
	m_inventoryText.setFillColor(sf::Color::White);
	m_inventoryText.setPosition(sf::Vector2f(10.f, 10.f));
		
	m_promptText.setCharacterSize(22);
	m_promptText.setFillColor(sf::Color::Yellow);

	m_inspectorText.setCharacterSize(16);
	m_inspectorText.setFillColor(sf::Color::Cyan);
	m_inspectorText.setPosition(sf::Vector2f(Globals::SCREEN_WIDTH - 220.f, 10.f));

	// Setup camera
	m_camera.setSize(sf::Vector2f(static_cast<float>(Globals::SCREEN_WIDTH),
								  static_cast<float>(Globals::SCREEN_HEIGHT))); // Matches window size
	m_camera.setCenter(m_player.getWorldPosition());

	m_resourceRegistry.initializeResources();
	m_machineRegistry.initializeMachines();

	m_tileVisualizer.registerDefaults();
}

// Will be used to determine player key presses and click inputs during gameplay
void Gameplay::HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window)
{
	if (!t_event.has_value()) return;

	if (t_event->is<sf::Event::KeyPressed>())
	{
		const auto* keyPress = t_event->getIf<sf::Event::KeyPressed>();
		if (keyPress && keyPress->code == sf::Keyboard::Key::Escape)
		{
			m_pendingAction = SceneActions::SwitchToMenu;
		}
		if (keyPress && keyPress->code == sf::Keyboard::Key::F1)
		{
			m_tileMap.toggleDebugElevation();
		}
	}

	// We are investigating key releases here - i.e. out of loop
	m_placementController.handleEvent(t_event.value());
}

void Gameplay::Update(sf::Time t_dt)
{
	/*--
	ToDo: Separate Later on into Paused & Unpaused will help clarify operations
	--*/

	m_player.update(t_dt, m_tileMap, m_resourceRegistry);

	// Update camera to follow player
	updateCamera();

	/// ToDo: Prevent constant checks, when we add in modes for this and not
	/// keybinds we can do this a lot easier but for now it is constant
	PlacementRequest l_request = m_placementController.update(
		m_player.getWorldPosition(),
		m_player.getFacing(),
		m_machineSystem
	);

	// Check Intent for actions
	switch (l_request.m_intent)
	{
	case PlacementIntent::Place:
		if (l_request.m_valid)
			m_machineSystem.placeMachine(
				l_request.m_definitionID,
				l_request.m_gridPos,
				l_request.m_worldOffset,
				l_request.m_facingDirection,
				m_tileMap);
		break;
	case PlacementIntent::Remove:
		if (l_request.m_valid)
			m_machineSystem.removeMachine(l_request.m_gridPos, m_tileMap);
		break;
	case PlacementIntent::None:
		break;
	}


	m_machineSystem.update(t_dt);

	m_areaText.setString("Biome " + getCurrentBiome());
}

void Gameplay::Render(sf::RenderWindow& t_window)
{
	t_window.setView(m_camera);

	m_tileMap.render(t_window, m_tileAccessor);

	if (m_tileMap.getDebugElevation())
		m_tileMap.renderDebugElevation(t_window, m_gameFont);

	m_player.render(t_window);

	/// ToDo: Prevent constant checks, when we add in modes for this and not
	/// keybinds we can do this a lot easier but for now it is constant
	m_placementController.render(t_window);

	t_window.setView(t_window.getDefaultView());

	t_window.draw(m_areaText);

	renderInventory(t_window);
	renderMachineInspector(t_window); 
	renderInteractionPrompt(t_window);
}

SceneActions Gameplay::getRequestedAction()
{
	SceneActions action = m_pendingAction;
	m_pendingAction = SceneActions::None;
	return action;
}


void Gameplay::updateCamera()
{
	m_camera.setCenter(m_player.getWorldPosition());

	// Clamping camera so it doesn't show outside world bounds
	float l_halfWidth = m_camera.getSize().x / 2.f;
	float l_halfHeight = m_camera.getSize().y / 2.f;

	float l_worldWidth = m_tileMap.getWidth() * Globals::TILE_SIZE;
	float l_worldHeight = m_tileMap.getHeight() * Globals::TILE_SIZE;

	sf::Vector2f l_camPos = m_camera.getCenter();

	// Clamp camera to world bounds
	if (l_camPos.x - l_halfWidth < 0.f)
		l_camPos.x = l_halfWidth;
	if (l_camPos.x + l_halfWidth > l_worldWidth)
		l_camPos.x = l_worldWidth - l_halfWidth;
	if (l_camPos.y - l_halfHeight < 0.f)
		l_camPos.y = l_halfHeight;
	if (l_camPos.y + l_halfHeight > l_worldHeight)
		l_camPos.y = l_worldHeight - l_halfHeight;

	m_camera.setCenter(l_camPos);
}

std::string Gameplay::getCurrentBiome() 
{
	sf::Vector2f l_playerPos = m_player.getWorldPosition();
	int l_chunkX = static_cast<int>(l_playerPos.x / Globals::TILE_SIZE) / Globals::CHUNK_SIZE;
	int l_chunkY = static_cast<int>(l_playerPos.y / Globals::TILE_SIZE) / Globals::CHUNK_SIZE;

	if (!m_chunkManager.isValidChunkPos(l_chunkX, l_chunkY))
		return "Unknown";

	switch (m_chunkManager.getChunk(l_chunkX, l_chunkY).getBiome())
	{
	case BiomeType::Plains:
		return "Plains";
	case BiomeType::Forest:
		return "Forest";
	case BiomeType::Mountains:
		return "Mountains";
	case BiomeType::Ocean:
		return "Ocean";
	case BiomeType::Desert:
		return "Desert";
	default: 
		return "Unassigned";
	}

}

// ToDo: Refactor into a proper space - not gameplay
void Gameplay::renderInventory(sf::RenderWindow& t_window)
{
	const std::vector<InventorySlot>& l_slots = m_player.getInventory().getSlots();
	std::string l_display = "-- Inventory --\n\n";

	for (const InventorySlot& l_slot : l_slots)
	{
		if (l_slot.m_isEmpty)
			continue;

		const ResourceDefinition* l_def = m_resourceRegistry.getResource(l_slot.m_resourceID);
		if (!l_def)
			continue;

		l_display += l_def->m_name + " x" + std::to_string(l_slot.m_stackCount) + "\n";
	}

	m_inventoryText.setString(l_display);
	t_window.draw(m_inventoryText);

}



void Gameplay::renderInteractionPrompt(sf::RenderWindow& t_window)
{
	// Get the tile the player is facing
	sf::Vector2f l_harvestPoint = m_player.getWorldPosition() +
		(m_player.getFacing() * Globals::HARVEST_REACH);

	sf::Vector2i l_targetGrid = m_tileMap.worldToGrid(l_harvestPoint);
	Tile* l_tile = m_tileMap.getTileAt(l_targetGrid.x, l_targetGrid.y);

	if (!l_tile || !l_tile->m_resource.isHarvestable())
		return;

	std::string l_prompt;
	switch (l_tile->m_resource.m_interactionType)
	{
		case InteractionType::Chop:  l_prompt = "[E] Chop";  break;
		case InteractionType::Mine:  l_prompt = "[E] Mine";  break;
		case InteractionType::Shovel: l_prompt = "[E] Dig"; break;
		default: return;
	}

	// Position prompt at center bottom of screen
	m_promptText.setString(l_prompt);
	m_promptText.setPosition(sf::Vector2f(
		Globals::SCREEN_WIDTH / 2.f - m_promptText.getGlobalBounds().size.x / 2.f,
		Globals::SCREEN_HEIGHT - 80.f
	));

	t_window.draw(m_promptText);
}

void Gameplay::renderMachineInspector(sf::RenderWindow& t_window)
{
	sf::Vector2f l_harvestPoint = m_player.getWorldPosition() + 
		(m_player.getFacing() * Globals::HARVEST_REACH); 

	sf::Vector2i l_targetGrid = m_tileMap.worldToGrid(l_harvestPoint); 

	MachineComponent* l_machine = m_machineSystem.getMachineAt(l_targetGrid);

	if (!l_machine)
		return;

	const MachineDefinition* l_def =
		m_machineRegistry.getMachine(l_machine->getDefinitionId());

	std::string l_display = "-- " +
		(l_def ? l_def->m_name : "Machine") + " --\n";

	l_display += l_machine->isIdle()
		? "Status: Idle\n"
		: "Status: Active\n";

	l_display += "\nOutput:\n";

	const auto& l_slots = l_machine->getOutputInventory().getSlots();
	bool l_empty = true;

	for (const auto& l_slot : l_slots) 
	{
		if (!l_slot.m_isEmpty) 
		{
			const ResourceDefinition* l_resDef = 
				m_resourceRegistry.getResource(l_slot.m_resourceID); 
			l_display += (l_resDef ? l_resDef->m_name : "Unknown") 
				+ " x" + std::to_string(l_slot.m_stackCount) + "\n"; 
			l_empty = false; 
		}
	}

	if (l_empty)
		l_display += "Empty\n";

	if (l_machine->getInputBufferCount() > 0)
	{
		l_display += "\nInput buffers:\n";

		for (int i = 0; i < l_machine->getInputBufferCount(); i++)
		{
			const auto& l_bufferSlots =
				l_machine->getInputBuffer(i).getSlots();

			for (const auto& l_slot : l_bufferSlots)
			{
				if (!l_slot.m_isEmpty)
				{
					const ResourceDefinition* l_resDef =
						m_resourceRegistry.getResource(l_slot.m_resourceID);

					l_display += "Ch" + std::to_string(i) + ": "
						+ (l_resDef ? l_resDef->m_name : "Unknown")
						+ " x" + std::to_string(l_slot.m_stackCount)
						+ "\n";
				}
			}
		}
	}

	m_inspectorText.setString(l_display);
	t_window.draw(m_inspectorText);
}
