#include "Gameplay.h"

Gameplay::Gameplay(sf::RenderWindow& t_window) 
	: m_pendingAction(SceneActions::None)
	, m_player(sf::Vector2f((64.f * 64.f, 64.f * 64.f), 16.f))
	, m_tileMap(Globals::WORLD_WIDTH, Globals::WORLD_HEIGHT, Globals::TILE_SIZE)
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
	m_chunkManager.initialize(5878514, m_tileMap, &l_loadingScreen);	// debugging ; Const seed: 5878514

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

	// Setup camera
	m_camera.setSize(sf::Vector2f(static_cast<float>(Globals::SCREEN_WIDTH),
								  static_cast<float>(Globals::SCREEN_HEIGHT))); // Matches window size
	m_camera.setCenter(m_player.getWorldPosition());

	m_resourceRegistry.initializeResources();

	// Debug Remove Later
	const ResourceDefinition* l_woodDef = m_resourceRegistry.getResource(1);

	if (l_woodDef)
	{
		int l_leftOver = m_player.getInventory().addResources(1, 200, l_woodDef->m_maxInStack);
		std::cout << "Gameplay: Added " << l_woodDef->m_name << " to player inventory. Leftover: " << l_leftOver << "\n";
		std::cout << "Gameplay: Player wood count: " << m_player.getInventory().getResourceCount(1) << "\n";
	}
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
}

void Gameplay::Update(sf::Time t_dt)
{
	/*--
	ToDo: Separate Later on into Paused & Unpaused will help clarify operations
	--*/

	m_player.update(t_dt, m_tileMap, m_resourceRegistry);

	// Update camera to follow player
	updateCamera();

	m_areaText.setString("Biome " + getCurrentBiome());
}

void Gameplay::Render(sf::RenderWindow& t_window)
{
	t_window.setView(m_camera);

	m_tileMap.render(t_window);

	if (m_tileMap.getDebugElevation())
		m_tileMap.renderDebugElevation(t_window, m_gameFont);

	m_player.render(t_window);

	t_window.setView(t_window.getDefaultView());

	t_window.draw(m_areaText);
	renderInventory(t_window);
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
