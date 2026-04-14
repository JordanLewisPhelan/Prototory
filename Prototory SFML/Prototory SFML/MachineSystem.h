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

	
	bool placeMachine(uint32_t t_machineID,
					sf::Vector2i t_gridPos,
					sf::Vector2f t_worldOffset,
					sf::Vector2f t_facingDirection,
					TileMap& t_tileMap);

	bool removeMachine(sf::Vector2i t_gridPos,
					   TileMap& t_tileMap);

	
	bool hasMachineAt(sf::Vector2i t_gridPos) const;
	MachineComponent* getMachineAt(sf::Vector2i t_gridPos) const;

private:
	TileAccessor& m_tileAccessor;
	MachineRegistry& m_machineRegistry;
	const ResourceRegistry& m_resourceRegistry;		
	std::vector<sf::Vector2i> m_activeTiles;		

	

	
	void tickComponent(MachineComponent& t_component, const MachineDefinition& t_definition, sf::Time t_dt, sf::Vector2i t_gridPos, const ResourceRegistry& t_resourceRegistry);

	
	void transferComponent(MachineComponent& t_component, const MachineDefinition& t_definition, sf::Time t_dt, sf::Vector2i t_gridPos);

	

	
	std::vector<sf::Vector2i> getDirectionalOffsets(const MachineDefinition& t_def) const;

	
	sf::Vector2i dominantTileStep(sf::Vector2f t_direction) const;

	
	void resolveConnection(sf::Vector2i t_gridPos);

	
	void notifyNeighbours(sf::Vector2i t_gridPos);

	
	void drainInputBuffers(MachineComponent& t_component,
		const MachineDefinition& t_definition);
};