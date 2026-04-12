#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "MachineComponent.h"
#include "MachineRegistry.h"
#include "TileAccessor.h"
#include "TileMap.h"
#include "ChunkManager.h"
#include "ResourceRegistry.h"
#include <iostream>


class MachineSystem
{
public:
	MachineSystem(TileAccessor& t_tileAccessor, MachineRegistry& t_machineRegistry, const ResourceRegistry& t_resourceRegistry);


	void update(sf::Time t_dt);

	// - Placement & Removal -
	bool placeMachine(uint32_t t_machineID,
					sf::Vector2i t_gridPos,
					sf::Vector2f t_worldOffset,
					sf::Vector2f t_facingDirection,
					TileMap& t_tileMap);

	bool removeMachine(sf::Vector2i t_gridPos,
					   TileMap& t_tileMap);

	// - Existence Assurance -
	bool hasMachineAt(sf::Vector2i t_gridPos) const;
	MachineComponent* getMachineAt(sf::Vector2i t_gridPos) const;


private:
	TileAccessor& m_tileAccessor;
	MachineRegistry& m_machineRegistry;
	const ResourceRegistry& m_resourceRegistry;		// Read only version of the resourceRegistry
	std::vector<sf::Vector2i> m_activeTiles;		// ToDo; After confirming logic works update into a circuit graph - this is just a compilation of machineTiles

	/// - Tick Updates -

	// Machine process to update its tick as long as it has a tick rate
	void tickComponent(MachineComponent& t_component, const MachineDefinition& t_definition, sf::Time t_dt, sf::Vector2i t_gridPos, const ResourceRegistry& t_resourceRegistry);

	// Process to separate transfer ticks and logic from main loop; is ran as long as machine has a transfer rate
	void transferComponent(MachineComponent& t_component, const MachineDefinition& t_definition, sf::Time t_dt, sf::Vector2i t_gridPos);


	/// - Neighbour Influenced logic -

	// Checks available and valid neighbours - ToDo: Use Definition as a directional allowance flag
	std::vector<sf::Vector2i> getDirectionalOffsets(const MachineDefinition& t_def) const;

	// Returns the direction closest to the direction the machine is facing
	sf::Vector2i dominantTileStep(sf::Vector2f t_direction) const;

	// Checks its positional neighbours and determines whether connections should be made when a machine is placed
	void resolveConnection(sf::Vector2i t_gridPos);

	// Tells neighbours re-evaluate their connections - primary use is when a given machine is removed
	void notifyNeighbours(sf::Vector2i t_gridPos);


	// Transfers buffer content into inventories of components
	void drainInputBuffers(MachineComponent& t_component,
		const MachineDefinition& t_definition);
};