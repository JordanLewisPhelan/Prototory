#include "MachineComponent.h"

MachineComponent::MachineComponent(const MachineDefinition& t_definition, sf::Vector2f t_worldOffset, sf::Vector2f t_facingDirection)
	: m_defID(t_definition.m_id)
	, m_worldOffset(t_worldOffset)
	, m_facingDirection(t_facingDirection)
	, m_tickTimer(0.f)
	, m_transferTimer(0.f)
	, m_outputTarget(std::nullopt)
	, m_idle(false)
	, m_outputInventory(t_definition.m_outputSlots)
{
	for (int i = 0; i < t_definition.m_inputChannels; i++)
	{
		m_inputBuffers.emplace_back(t_definition.m_inputSlots);
	}

	
	switch (t_definition.m_purpose)
	{
		case MachinePurpose::Harvester:
			m_visual.m_colour = sf::Color::Red;   
			break;
		case MachinePurpose::Transporter:
			m_visual.m_colour = sf::Color::Black;  
			break;
		case MachinePurpose::Storage:
			m_visual.m_colour = sf::Color(139, 90, 43);  
			break;
		default:
			m_visual.m_colour = sf::Color(160, 160, 160); 
			break;
	}

	
	m_visual.m_idle = false;
	m_visual.m_size = { Globals::TILE_SIZE * 0.75f, Globals::TILE_SIZE * 0.5f };
	m_visual.m_offsetPos = t_worldOffset;
}

const MachineVisual& MachineComponent::getVisual() const
{
	return m_visual;
}

std::optional<sf::Vector2i> MachineComponent::getOutputTarget() const
{
	return m_outputTarget;
}

sf::Vector2f MachineComponent::getFacingDirection() const
{
	return m_facingDirection;
}

int MachineComponent::getInputBufferCount() const
{
	return static_cast<int>(m_inputBuffers.size());
}

const std::vector<sf::Vector2i>& MachineComponent::getInputSources() const
{
	return m_inputSources;
}

void MachineComponent::setIdle(bool t_idle)
{
	m_idle = t_idle;
	m_visual.m_idle = t_idle;
}

Inventory& MachineComponent::getOutputInventory()
{
	return m_outputInventory;
}

Inventory& MachineComponent::getInputBuffer(int t_channel)
{
	if (t_channel < 0 || t_channel >= static_cast<int>(m_inputBuffers.size()))
	{
		std::cerr << "MachineComponent: invalid channel index " << t_channel << "\n";
		throw std::out_of_range("MachineComponent::getInputBuffer invalid channel");
	}
	return m_inputBuffers[t_channel];
}

void MachineComponent::setOutputTarget(sf::Vector2i t_target)
{
	m_outputTarget = t_target;
}

void MachineComponent::clearOutputTarget()
{
	m_outputTarget = std::nullopt;
}

void MachineComponent::addInputSource(sf::Vector2i t_source)
{
	m_inputSources.push_back(t_source);
}

void MachineComponent::removeInputSource(sf::Vector2i t_source)
{
	m_inputSources.erase(std::remove(
		m_inputSources.begin(), m_inputSources.end(), t_source
	),
		m_inputSources.end()
	);
}

bool MachineComponent::hasInputSource(sf::Vector2i t_source)
{
	return std::find(m_inputSources.begin(), m_inputSources.end(), t_source) != m_inputSources.end();

}

void MachineComponent::advanceTickTimer(sf::Time t_dt)
{
	m_tickTimer += t_dt.asSeconds();
}

float MachineComponent::getTickTimer() const
{
	return m_tickTimer;
}

void MachineComponent::resetTickTimer()
{
	m_tickTimer = 0.f;
}

void MachineComponent::advanceTransferTimer(sf::Time t_dt)
{
	m_transferTimer += t_dt.asSeconds();
}

float MachineComponent::getTransferTimer() const
{
	return m_transferTimer;
}

void MachineComponent::resetTransferTimer()
{
	m_transferTimer = 0.f;
}

