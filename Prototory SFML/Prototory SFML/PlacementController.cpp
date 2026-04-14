#include "PlacementController.h"

PlacementController::PlacementController(const MachineRegistry& t_registry, TileAccessor& t_tileAccessor, const sf::Font& t_font)
	: m_machineRegistry(t_registry)
	, m_tileAccessor(t_tileAccessor)
	, m_confirmPlace(false)
	, m_confirmRemove(false)
	, m_placementValid(false)
	, m_selectionLabel(t_font)
{
	m_highlightShape.setOutlineThickness(1.5f);

	m_selectionLabel.setCharacterSize(18);
	m_selectionLabel.setFillColor(sf::Color::White);
	m_selectionLabel.setOutlineColor(sf::Color::Black);
	m_selectionLabel.setOutlineThickness(2.f);
}

PlacementRequest PlacementController::update(const sf::Vector2f& t_playerPos, const sf::Vector2f& t_playerFacing, const MachineSystem& t_machineSystem)
{
	PlacementRequest l_request;

	m_targetTile = determineHighlightTile(t_playerPos, t_playerFacing);

	if (!m_targetTile.has_value())
	{
		m_confirmPlace = false;
		m_confirmRemove = false;
		return l_request; 
	}

	sf::Vector2i l_gridPos = m_targetTile.value();

	l_request.m_gridPos = l_gridPos;
	l_request.m_facingDirection = t_playerFacing; 

	
	if (m_confirmRemove)
	{
		l_request.m_intent = PlacementIntent::Remove;
		l_request.m_valid = m_tileAccessor.isValid(l_gridPos) &&
							t_machineSystem.hasMachineAt(l_gridPos);
		m_confirmRemove = false;
		m_confirmPlace = false;

		return l_request;
	}

	
	if (!m_selectedDefinitionID.has_value())
	{
		m_confirmPlace = false;
		return l_request;
	}

	uint32_t l_defID = m_selectedDefinitionID.value();
	const MachineDefinition* l_def = m_machineRegistry.getMachine(l_defID);

	if (!l_def)
	{
		m_confirmPlace = false;
		return l_request;
	}

	m_placementValid = checkValidity(l_gridPos, *l_def, t_machineSystem);

	l_request.m_intent = PlacementIntent::Place;
	l_request.m_definitionID = l_defID;
	l_request.m_valid = m_placementValid && m_confirmPlace;
	l_request.m_worldOffset = determineHighlightOffset(t_playerFacing);

	m_confirmPlace = false;

	return l_request;
}

void PlacementController::render(sf::RenderWindow& t_window)
{
	if (!m_selectedDefinitionID.has_value() || !m_targetTile.has_value())
		return;

	const MachineDefinition* l_def = m_machineRegistry.getMachine(m_selectedDefinitionID.value());

	if (!l_def)
		return;

	sf::Vector2i l_gridPos = m_targetTile.value();

	
	float l_tileSize = static_cast<float>(Globals::TILE_SIZE);

	sf::Vector2f l_size(l_tileSize * 0.75f, l_tileSize * 0.5f);
	m_highlightShape.setSize(l_size);

	
	
	sf::Color l_fill = m_placementValid
		? sf::Color(100, 220, 100, 120)
		: sf::Color(220, 100, 100, 120);

	sf::Color l_outline = m_placementValid
		? sf::Color(100, 220, 100, 220)
		: sf::Color(220, 100, 100, 220);

	m_highlightShape.setFillColor(l_fill);
	m_highlightShape.setOutlineColor(l_outline);

	
	sf::Vector2f l_offset = determineHighlightOffset(sf::Vector2f(0.f, 0.f));
	sf::Vector2f l_centre(
		l_gridPos.x * l_tileSize + (l_tileSize - l_size.x) / 2.f,
		l_gridPos.y * l_tileSize + (l_tileSize - l_size.y) / 2.f
	);

	m_highlightShape.setPosition(l_centre + l_offset);

	t_window.draw(m_highlightShape);

	if (m_selectedDefinitionID.has_value())
	{
		const MachineDefinition* l_def =
			m_machineRegistry.getMachine(m_selectedDefinitionID.value());
		if (l_def)
		{
			m_selectionLabel.setString(l_def->m_name);
			
			m_selectionLabel.setPosition(sf::Vector2f(
				l_gridPos.x * l_tileSize,
				l_gridPos.y * l_tileSize - 20.f
			));
			t_window.draw(m_selectionLabel);
		}
	}
}

void PlacementController::handleEvent(const sf::Event& t_event)
{
	if (const auto* l_keyReleased = t_event.getIf<sf::Event::KeyReleased>())
	{
		
		auto l_selectOrToggle = [&](uint32_t t_id)
			{
				if (m_selectedDefinitionID.has_value() &&
					m_selectedDefinitionID.value() == t_id)
				{
					m_selectedDefinitionID = std::nullopt; 
				}
				else
				{
					m_selectedDefinitionID = t_id;
				}
			};

		
		
		switch (l_keyReleased->code)
		{
			case sf::Keyboard::Key::Num1: l_selectOrToggle(1); break;
			case sf::Keyboard::Key::Num2: l_selectOrToggle(2); break;
			case sf::Keyboard::Key::Num3: l_selectOrToggle(3); break;
			case sf::Keyboard::Key::Num4: l_selectOrToggle(4); break;
			default: break;
		}
	}

	
	if (const auto* l_mouseRelease = t_event.getIf<sf::Event::MouseButtonReleased>())
	{
		if (l_mouseRelease->button == sf::Mouse::Button::Left)
		{
			m_confirmPlace = true;
		}
		if (l_mouseRelease->button == sf::Mouse::Button::Right)
		{
			m_confirmRemove = true;
		}
	}
}

bool PlacementController::hasSelection() const
{
	return m_selectedDefinitionID.has_value();
}

uint32_t PlacementController::getSelectedId() const
{
	return m_selectedDefinitionID.value_or(0);
}

sf::Vector2i PlacementController::determineHighlightTile(const sf::Vector2f& t_playerPos, const sf::Vector2f& t_playerFacing) const
{
	
	sf::Vector2f l_targetWorld(
		t_playerPos.x + t_playerFacing.x * Globals::TILE_SIZE,
		t_playerPos.y + t_playerFacing.y * Globals::TILE_SIZE
	);

	return sf::Vector2i(
		static_cast<int>(l_targetWorld.x / Globals::TILE_SIZE),
		static_cast<int>(l_targetWorld.y / Globals::TILE_SIZE)
	);
}

sf::Vector2f PlacementController::determineHighlightOffset(const sf::Vector2f& t_playerFacing) const
{
	
	float l_shift = Globals::TILE_SIZE * 0.15f;

	return sf::Vector2f(
		-t_playerFacing.x * l_shift,
		-t_playerFacing.y * l_shift
	);
}

bool PlacementController::checkValidity(sf::Vector2i t_gridPos, const MachineDefinition& t_def, const MachineSystem& t_machineSystem) const
{
	if (!m_tileAccessor.isValid(t_gridPos))
		return false;

	if (t_machineSystem.hasMachineAt(t_gridPos))
		return false;

	
	if (t_def.m_purpose == MachinePurpose::Harvester)
	{
		int l_range = t_def.m_harvestRange;

		for (int dx = -l_range; dx <= l_range; dx++)
		{
			for (int dy = l_range; dy <= l_range; dy++)
			{
				
				
				if (l_range > 0 && dx == 0 && dy == 0)
					continue;

				sf::Vector2i l_checkPos = t_gridPos + sf::Vector2i(dx, dy);
				Tile* l_tile = m_tileAccessor.get(l_checkPos);

				if (l_tile &&
					l_tile->m_resource.isHarvestable() &&
					l_tile->m_resource.m_interactionType == t_def.m_harvestType)
					return true;
			}
		}
		return false;	
	}

	
	
	return true;
}
