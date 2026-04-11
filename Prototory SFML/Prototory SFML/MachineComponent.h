#pragma once

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Inventory.h"
#include "MachineDefinition.h"
#include "Globals.h"



struct MachineVisual {
	sf::Color m_colour;
	sf::Vector2f m_offsetPos;	// allows positional variance with a tile - to feel more natural
	sf::Vector2f m_size;
	bool m_idle;
};


class MachineComponent
{
public:
	MachineComponent(uint32_t t_defID, 
		const MachineDefinition& t_definition,
		sf::Vector2f t_worldOffset,
		sf::Vector2f t_facingDirection);

	/// - Accessors -
	const MachineVisual& getVisual() const;
	std::optional<sf::Vector2i> getOutputTarget() const;
	sf::Vector2f getFacingDirection() const;
	uint32_t getDefinitionId() const { return m_defID; }
	int  getInputBufferCount() const;
	const std::vector<sf::Vector2i>& getInputSources() const;


	/// - Queriers -
	bool isIdle() const { return m_idle; } 
	void setIdle(bool t_idle);
	Inventory& getOutputInventory();
	Inventory& getInputBuffer(int t_channel);


	/// - Input/Output -
	void setOutputTarget(sf::Vector2i t_target);
	void clearOutputTarget();
	void addInputSource(sf::Vector2i t_source);
	void removeInputSource(sf::Vector2i t_source);
	bool hasInputSource(sf::Vector2i t_source);


	/// - Tick Timers -
	void  advanceTickTimer(sf::Time t_dt);
	float getTickTimer() const;
	void  resetTickTimer();
	void  advanceTransferTimer(sf::Time t_dt);
	float getTransferTimer() const;
	void  resetTransferTimer();


private:
	/* - Variables - */

	/// -General data information-
	uint32_t m_defID;
	float m_tickTimer;
	float m_transferTimer;
	sf::Vector2f m_worldOffset;
	sf::Vector2f m_facingDirection;

	/// -Connectivity & World data-
	std::optional<sf::Vector2i> m_outputTarget;				// Grid space location of the tile we are facing-outputting/sending to
	std::vector<sf::Vector2i> m_inputSources;	// Vector containing our valid data brokers via their Grid Positions

	/// -Inventories-
	std::vector<Inventory> m_inputBuffers;		// One per input channel/source
	Inventory m_outputInventory;				// Inventory to query when attempting to move data

	/// -State & Displays-
	bool m_idle;
	MachineVisual m_visual;
};