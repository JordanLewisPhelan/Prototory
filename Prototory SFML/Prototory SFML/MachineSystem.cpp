#include "MachineSystem.h"



MachineSystem::MachineSystem(TileAccessor& t_tileAccessor, MachineRegistry& t_machineRegistry, const ResourceRegistry& t_resourceRegistry)
	: m_tileAccessor(t_tileAccessor)
	, m_machineRegistry(t_machineRegistry)
	, m_resourceRegistry(t_resourceRegistry)
{
}



void MachineSystem::update(sf::Time t_dt)
{
	// Loop through all tiles that are confirmed in the world
	/// ToDo: Update to be Graph based and ordered - prevents checking whole system each
	/// frame when we can cut off further processing of elements that cant be processing
	for (const sf::Vector2i l_pos : m_activeTiles)
	{

		Tile* l_tile = m_tileAccessor.get(l_pos);

		// Make sure tile position is valid
		if (!l_tile)
		{
			std::cerr << "MachineSystem: active tile at "
					  << l_pos.x << "," << l_pos.y
					  << " returned nullptr - desync in active list\n";
			continue;
		}

		// Checks to ensure each tile here has a machine component 
		// before trying to work with it
		if (!l_tile->m_machine.has_value())
		{
			std::cerr << "MachineSystem: tile at "
					  << l_pos.x << "," << l_pos.y
					  << " has no machine component - desync\n";
			continue;
		}

		MachineComponent& l_component = l_tile->m_machine.value();

		const MachineDefinition* l_def =
			m_machineRegistry.getMachine(l_component.getDefinitionId());

		// Make sure machine definition intended to be used actually exists
		if (!l_def)
		{
			std::cerr << "MachineSystem: no definition found for ID "
					  << l_component.getDefinitionId() << " at "
					  << l_pos.x << "," << l_pos.y << "\n";
			continue;
		}

		// Do Machine update operations
		tickComponent(l_component, *l_def, t_dt, l_pos, m_resourceRegistry);
		transferComponent(l_component, *l_def, t_dt, l_pos);
		drainInputBuffers(l_component, *l_def);
	}
}

bool MachineSystem::placeMachine(uint32_t t_machineID, sf::Vector2i t_gridPos, sf::Vector2f t_worldOffset, sf::Vector2f t_facingDirection, TileMap& t_tileMap)
{
	// Checking Tile Space - Can quickly and easily Update to check for player range allowances later
	if (!m_tileAccessor.isValid(t_gridPos))
	{
		std::cerr << "MachineSystem: placeMachine - invalid grid position "
				  << t_gridPos.x << "," << t_gridPos.y << "\n";
		return false;
	}


	// Ensure a valid definition before continuing
	const MachineDefinition* l_def = m_machineRegistry.getMachine(t_machineID);

	if (!l_def)
	{
		std::cerr << "MachineSystem: placeMachine - unknown definition ID "
				  << t_machineID << "\n";
		return false;
	}


	// Get the tile for further queries
	Tile* l_tile = m_tileAccessor.get(t_gridPos);

	// Check to ensure the Tile has no existing machine on it
	if (l_tile->m_machine.has_value())
	{
		std::cerr << "MachineSystem: placeMachine - tile "
				  << t_gridPos.x << "," << t_gridPos.y
				  << " already occupied\n";
		return false;
	}

	// Make the Machine and assign to tile directly
	l_tile->m_machine = MachineComponent(*l_def, t_worldOffset, t_facingDirection);

	Tile* l_mapTile = t_tileMap.getTileAt(t_gridPos.x, t_gridPos.y);

	if (l_mapTile)
		l_mapTile->m_machine = l_tile->m_machine;

	// Add to the tiles to update
	m_activeTiles.push_back(t_gridPos);

	resolveConnection(t_gridPos);


	std::vector<sf::Vector2i> l_offsets = {
	{ 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 }
	};

	for (const sf::Vector2i& l_offset : l_offsets)
	{
		sf::Vector2i l_neighbourPos = t_gridPos + l_offset;
		if (m_tileAccessor.isValid(l_neighbourPos) &&
			getMachineAt(l_neighbourPos) != nullptr)
		{
			resolveConnection(l_neighbourPos);
		}
	}


	std::cout << "MachineSystem: placed " << l_def->m_name
			  << " at " << t_gridPos.x << "," << t_gridPos.y << "\n";

	return true;
}

bool MachineSystem::removeMachine(sf::Vector2i t_gridPos, TileMap& t_tileMap)
{
	if (!m_tileAccessor.isValid(t_gridPos))
	{
		std::cerr << "MachineSystem: removeMachine - invalid grid position "
			<< t_gridPos.x << "," << t_gridPos.y << "\n";
		return false;
	}

	Tile* l_tile = m_tileAccessor.get(t_gridPos);

	// Check to ensure the Tile has no existing machine on it
	if (!l_tile->m_machine.has_value())
	{
		std::cerr << "MachineSystem: removeMachine - tile "
			<< t_gridPos.x << "," << t_gridPos.y
			<< " no machine present\n";
		return false;
	}

	// Ensures local negighbours dont hold outdated references/data
	notifyNeighbours(t_gridPos);

	// Clear the std::optional machine of the tile
	l_tile->m_machine.reset();

	Tile* l_mapTile = t_tileMap.getTileAt(t_gridPos.x, t_gridPos.y);

	if (l_mapTile)
		l_mapTile->m_machine.reset();

	// Remove that tile from the list of machinery tiles
	m_activeTiles.erase(
		std::remove(m_activeTiles.begin(), m_activeTiles.end(), t_gridPos), m_activeTiles.end()
	);

	std::cout << "MachineSystem: removed machine at "
		<< t_gridPos.x << "," << t_gridPos.y << "\n";

	return true;
}



bool MachineSystem::hasMachineAt(sf::Vector2i t_gridPos) const
{
	Tile* l_tile = m_tileAccessor.get(t_gridPos);

	if (!l_tile)
		return false;

	return l_tile->m_machine.has_value();
}

MachineComponent* MachineSystem::getMachineAt(sf::Vector2i t_gridPos) const
{
	Tile* l_tile = m_tileAccessor.get(t_gridPos);

	if (!l_tile || !l_tile->m_machine.has_value())
		return nullptr;

	return &l_tile->m_machine.value();
}



/* - Component Processors/Updators - */

void MachineSystem::tickComponent(MachineComponent& t_component, const MachineDefinition& t_definition, sf::Time t_dt, sf::Vector2i t_pos, const ResourceRegistry& t_resourceRegistry)
{
	// Non-ticking machines skip entirely
	if (t_definition.m_tickRate <= 0.f)
		return;

	t_component.advanceTickTimer(t_dt);

	if (t_component.getTickTimer() < t_definition.m_tickRate)
		return;

	t_component.resetTickTimer();

	// Only harvesters have tick logic currently
	if (t_definition.m_purpose != MachinePurpose::Harvester)
		return;

	// Output full - go idle, nothing to do
	if (t_component.getOutputInventory().isFull())
	{
		t_component.setIdle(true);
		return;
	}

	// Get the tile this machine sits on
	Tile* l_tile = m_tileAccessor.get(t_pos);
	if (!l_tile)
		return;

	// Check harvest range for valid tiles
	int l_range = t_definition.m_harvestRange;
	bool l_harvested = false;


	for (int dx = -l_range; dx <= l_range && !l_harvested; dx++)
	{
		for (int dy = -l_range; dy <= l_range && !l_harvested; dy++)
		{
			// Enforces we check local tile always unless we have range
			if (l_range > 0 && dx == 0 && dy == 0)
				continue;

			sf::Vector2i l_targetPos = t_pos + sf::Vector2i(dx, dy);


			if (!m_tileAccessor.isValid(l_targetPos))
				continue;

			Tile* l_targetTile = m_tileAccessor.get(l_targetPos);

			if (!l_targetTile)
				continue;

			TileResource& l_resource = l_targetTile->m_resource;

			//std::cout << "checking tile " << l_targetPos.x << "," << l_targetPos.y
			//	<< " harvestable: " << l_resource.isHarvestable()
			//	<< " quantity: " << l_resource.m_currentQuantity << "\n";

			// Check if this tile matches what this machine harvests
			if (!l_resource.isHarvestable())
				continue;

			if (l_resource.m_interactionType != t_definition.m_harvestType)
			{
				std::cout << "interaction mismatch: tile has "
					<< static_cast<int>(l_resource.m_interactionType)
					<< " def wants "
					<< static_cast<int>(t_definition.m_harvestType) << "\n";
				continue;
			}


			int l_amount = std::min(t_definition.m_harvestAmount,
									l_resource.m_currentQuantity);

			l_resource.m_currentQuantity -= l_amount;

			// Inideal but cleaner than alternative for now
			const ResourceDefinition* l_resDef = t_resourceRegistry.getResource(l_resource.m_resourceID);

			if (!l_resDef)
			{
				std::cerr << "MachineSystem: unknown resource ID "
						  << l_resource.m_resourceID << " during harvest\n";
				return;
			}

			// Get max stack from resource
			t_component.getOutputInventory().addResources(
				l_resource.m_resourceID,
				l_amount,
				l_resDef->m_maxInStack
			);


			t_component.setIdle(false);
			l_harvested = true;
		}
	}


	// Nothing harvestable found in range
	if (!l_harvested)
		t_component.setIdle(true);
}

void MachineSystem::transferComponent(MachineComponent& t_component, const MachineDefinition& t_definition, sf::Time t_dt, sf::Vector2i t_pos)
{
	if (t_definition.m_purpose != MachinePurpose::Transporter)
		return;

	// Non-transferring machines skip entirely
	if (t_definition.m_transferRate <= 0.f)
		return;

	t_component.advanceTransferTimer(t_dt);

	if (t_component.getTransferTimer() < t_definition.m_transferRate)
		return;

	t_component.resetTransferTimer();


	const std::vector<sf::Vector2i>& l_sources = t_component.getInputSources(); 

	for (const sf::Vector2i& l_sourcePos : l_sources)
	{
		MachineComponent* l_source = getMachineAt(l_sourcePos);
		if (!l_source)
			continue;

		if (l_source->getOutputInventory().isEmpty())
			continue;

		if (t_component.getOutputInventory().isFull())
			break;

		const auto& l_slots = l_source->getOutputInventory().getSlots();
		for (const auto& l_slot : l_slots)
		{
			if (!l_slot.m_isEmpty)
			{
				const ResourceDefinition* l_resDef =
					m_resourceRegistry.getResource(l_slot.m_resourceID);
				int l_maxStack = l_resDef ? l_resDef->m_maxInStack : 99;

				l_source->getOutputInventory().transferTo(
					t_component.getOutputInventory(),
					l_slot.m_resourceID,
					t_definition.m_transferAmount,
					l_maxStack
				);
				break;
			}
		}
	}

	// No output target - nothing to transfer to
	if (!t_component.getOutputTarget().has_value())
	{
		t_component.setIdle(true);
		return;
	}

	sf::Vector2i l_targetPos = t_component.getOutputTarget().value();

	// Validate target position
	if (!m_tileAccessor.isValid(l_targetPos))
	{
		t_component.clearOutputTarget();
		t_component.setIdle(true);
		return;
	}

	MachineComponent* l_target = getMachineAt(l_targetPos);

	if (!l_target)
	{
		// Target no longer exists - clear the link
		t_component.clearOutputTarget();
		t_component.setIdle(true);
		return;
	}

	if (t_component.getOutputInventory().isEmpty())
	{
		t_component.setIdle(true);
		return;
	}

	// Push to target's first available input buffer
	int l_availableChannel = -1;
	for (int l_i = 0; l_i < l_target->getInputBufferCount(); l_i++)
	{
		if (!l_target->getInputBuffer(l_i).isFull())
		{
			l_availableChannel = l_i;
			break;
		}
	}

	if (l_availableChannel == -1)
	{
		t_component.setIdle(true);
		return;
	}


	const auto& l_slots = t_component.getOutputInventory().getSlots();

	for (const auto& l_slot : l_slots)
	{
		if (!l_slot.m_isEmpty)
		{
			const ResourceDefinition* l_resDef =
				m_resourceRegistry.getResource(l_slot.m_resourceID);
			int l_maxStack = l_resDef ? l_resDef->m_maxInStack : 99;

			const MachineDefinition* l_targetDef =
				m_machineRegistry.getMachine(l_target->getDefinitionId());

			if (l_targetDef && l_targetDef->m_purpose == MachinePurpose::Transporter)
			{
				// Next machine is a conveyor - push into its output inventory
				t_component.getOutputInventory().transferTo(
					l_target->getOutputInventory(),
					l_slot.m_resourceID,
					t_definition.m_transferAmount,
					l_maxStack
				);
			}
			else
			{
				// Next machine is a chest or storage - push into input buffer
				t_component.getOutputInventory().transferTo(
					l_target->getInputBuffer(l_availableChannel),
					l_slot.m_resourceID,
					t_definition.m_transferAmount,
					l_maxStack
				);
			}

			t_component.setIdle(false);
			return;
		}
	}
}


std::vector<sf::Vector2i> MachineSystem::getDirectionalOffsets(const MachineDefinition& t_def) const
{
	// Cardinals always checked
	std::vector<sf::Vector2i> l_offsets =
	{
		{  0, -1 }, {  0,  1 },
		{ -1,  0 }, {  1,  0 }
	};

	// Diagonals added based on definition flags later
	// e.g. if t_def allows diagonal connections, append them here

	return l_offsets;
}

sf::Vector2i MachineSystem::dominantTileStep(sf::Vector2f t_direction) const
{
	if (std::abs(t_direction.x) >= std::abs(t_direction.y))
		return { t_direction.x > 0.f ? 1 : -1, 0 };
	else
		return { 0, t_direction.y > 0.f ? 1 : -1 };
}

void MachineSystem::resolveConnection(sf::Vector2i t_gridPos)
{
	Tile* l_tile = m_tileAccessor.get(t_gridPos);

	if (!l_tile || !l_tile->m_machine.has_value())
		return;

	MachineComponent& l_component = l_tile->m_machine.value();

	const MachineDefinition* l_def = m_machineRegistry.getMachine(l_component.getDefinitionId());

	if (!l_def)
		return;


	// Get the tile this conveyor is facing
	sf::Vector2i l_facingTile = t_gridPos +
		dominantTileStep(l_component.getFacingDirection());

	if (m_tileAccessor.isValid(l_facingTile))
	{
		Tile* l_facingTilePtr = m_tileAccessor.get(l_facingTile);
		if (l_facingTilePtr && l_facingTilePtr->m_machine.has_value())
		{
			if (!l_component.getOutputTarget().has_value())
			{
				l_component.setOutputTarget(l_facingTile);

				MachineComponent& l_facingMachine = l_facingTilePtr->m_machine.value();

				const auto& l_sources = l_facingMachine.getInputSources();

				bool l_alreadyLinked = std::find(
					l_sources.begin(), l_sources.end(), t_gridPos)
					!= l_sources.end();

				if (!l_alreadyLinked)
					l_facingMachine.addInputSource(t_gridPos);

				std::cout << "LINK OUT: " << t_gridPos.x << "," << t_gridPos.y
					<< " -> " << l_facingTile.x << "," << l_facingTile.y << "\n";
			}
		}
	}
	

	std::vector<sf::Vector2i> l_offsets = getDirectionalOffsets(*l_def);

	// Adds the offset to current Tile/Grid position so we dont have to move through whole map for comparison
	for (const sf::Vector2i& l_offset : l_offsets)
	{
		sf::Vector2i l_neighbourPos = t_gridPos + l_offset;

		// Skip the output target 
		if (l_component.getOutputTarget().has_value() &&
			l_component.getOutputTarget().value() == l_neighbourPos)
			continue;

		if (!m_tileAccessor.isValid(l_neighbourPos))
			continue;

		Tile* l_neighbourTile = m_tileAccessor.get(l_neighbourPos);

		if (!l_neighbourTile || !l_neighbourTile->m_machine.has_value())
			continue;

		MachineComponent& l_neighbour = l_neighbourTile->m_machine.value();

		const MachineDefinition* l_neighbourDef =
			m_machineRegistry.getMachine(l_neighbour.getDefinitionId());

		if (!l_neighbourDef)
			continue;

		bool l_shouldAddAsInput = false;

		if (l_neighbourDef->m_purpose != MachinePurpose::Transporter
			&& l_neighbourDef->m_outputSlots > 0)
		{
			// Harvester or other producer adjacent to this conveyor
			l_shouldAddAsInput = true;
		}
		else if (l_neighbourDef->m_purpose == MachinePurpose::Transporter)
		{
			// Neighbouring conveyor facing toward this tile
			sf::Vector2i l_neighbourFacing = l_neighbourPos +
				dominantTileStep(l_neighbour.getFacingDirection());

			if (l_neighbourFacing == t_gridPos)
			{
				l_shouldAddAsInput = true;

				// Also set that conveyor's output target if not set
				if (!l_neighbour.getOutputTarget().has_value())
				{
					l_neighbour.setOutputTarget(t_gridPos);
					std::cout << "MachineSystem: conveyor "
						<< l_neighbourPos.x << "," << l_neighbourPos.y
						<< " output -> "
						<< t_gridPos.x << "," << t_gridPos.y << "\n";
				}
			}
		}

		if (l_shouldAddAsInput)
		{
			const auto& l_sources = l_component.getInputSources();
			bool l_alreadyLinked = std::find(
				l_sources.begin(), l_sources.end(), l_neighbourPos)
				!= l_sources.end();

			if (!l_alreadyLinked)
			{
				l_component.addInputSource(l_neighbourPos);

				std::cout << "LINK IN: " << t_gridPos.x << "," << t_gridPos.y
					<< " <- " << l_neighbourPos.x << "," << l_neighbourPos.y << "\n";
			}
		}
	}
}

void MachineSystem::notifyNeighbours(sf::Vector2i t_gridPos)
{
	Tile* l_tile = m_tileAccessor.get(t_gridPos);

	if (!l_tile || !l_tile->m_machine.has_value())
		return;

	const MachineDefinition* l_def =
		m_machineRegistry.getMachine(l_tile->m_machine.value().getDefinitionId());

	if (!l_def)
		return;

	std::vector<sf::Vector2i> l_offsets = getDirectionalOffsets(*l_def);


	for (const sf::Vector2i l_offset : l_offsets)
	{
		sf::Vector2i l_neighbourPos = t_gridPos + l_offset;

		if (!m_tileAccessor.isValid(l_neighbourPos))
			continue;

		Tile* l_neighbourTile = m_tileAccessor.get(l_neighbourPos);

		if (!l_neighbourTile || !l_neighbourTile->m_machine.has_value())
			continue;

		MachineComponent& l_neighbour = l_neighbourTile->m_machine.value();

		// If neighbour was pointing at this position, clear its output 
		// as if this function is called a machine has been deleted at gridPos
		if (l_neighbour.getOutputTarget().has_value() &&
			l_neighbour.getOutputTarget().value() == t_gridPos)
		{
			l_neighbour.clearOutputTarget();
			std::cout << "MachineSystem: cleared output target for machine at "
					  << l_neighbourPos.x << "," << l_neighbourPos.y << "\n";
		}

		// Remove this position from neighbour's input sources
		if (l_neighbour.hasInputSource(t_gridPos))
		{
			l_neighbour.removeInputSource(t_gridPos);
			std::cout << "MachineSystem: cleared input source for machine at "
					  << l_neighbourPos.x << "," << l_neighbourPos.y << "\n";
		}
	}

}

void MachineSystem::drainInputBuffers(MachineComponent& t_component, const MachineDefinition& t_definition)
{
	// Only storage drains input buffers into output inventory
	if (t_definition.m_purpose != MachinePurpose::Storage)
		return;

	for (int i = 0; i < t_component.getInputBufferCount(); i++)
	{
		Inventory& l_buffer = t_component.getInputBuffer(i);

		if (l_buffer.isEmpty())
			continue;

		if (t_component.getOutputInventory().isFull())
			return;

		const auto& l_slots = l_buffer.getSlots();

		for (const auto& l_slot : l_slots)
		{
			if (!l_slot.m_isEmpty)
			{
				const ResourceDefinition* l_resDef =
					m_resourceRegistry.getResource(l_slot.m_resourceID);

				int l_maxStack = l_resDef ? l_resDef->m_maxInStack : 99;

				l_buffer.transferTo(
					t_component.getOutputInventory(),
					l_slot.m_resourceID,
					l_slot.m_stackCount,
					l_maxStack
				);
				break;
			}
		}
	}
}
