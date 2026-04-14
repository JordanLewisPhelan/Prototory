#pragma once

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Inventory.h"
#include "MachineDefinition.h"
#include "Globals.h"

struct MachineVisual {
	sf::Color m_colour;
	sf::Vector2f m_offsetPos;	
	sf::Vector2f m_size;
	bool m_idle;
};

class MachineComponent
{
public:
	MachineComponent(const MachineDefinition& t_definition,
		sf::Vector2f t_worldOffset,
		sf::Vector2f t_facingDirection);

	
	const MachineVisual& getVisual() const;
	std::optional<sf::Vector2i> getOutputTarget() const;
	sf::Vector2f getFacingDirection() const;
	uint32_t getDefinitionId() const { return m_defID; }
	int  getInputBufferCount() const;
	const std::vector<sf::Vector2i>& getInputSources() const;

	
	bool isIdle() const { return m_idle; } 
	void setIdle(bool t_idle);
	Inventory& getOutputInventory();
	Inventory& getInputBuffer(int t_channel);

	
	void setOutputTarget(sf::Vector2i t_target);
	void clearOutputTarget();
	void addInputSource(sf::Vector2i t_source);
	void removeInputSource(sf::Vector2i t_source);
	bool hasInputSource(sf::Vector2i t_source);

	
	void  advanceTickTimer(sf::Time t_dt);
	float getTickTimer() const;
	void  resetTickTimer();
	void  advanceTransferTimer(sf::Time t_dt);
	float getTransferTimer() const;
	void  resetTransferTimer();

private:
	

	
	uint32_t m_defID;
	float m_tickTimer;
	float m_transferTimer;
	sf::Vector2f m_worldOffset;
	sf::Vector2f m_facingDirection;

	
	std::optional<sf::Vector2i> m_outputTarget;				
	std::vector<sf::Vector2i> m_inputSources;	

	
	std::vector<Inventory> m_inputBuffers;		
	Inventory m_outputInventory;				

	
	bool m_idle;
	MachineVisual m_visual;
};