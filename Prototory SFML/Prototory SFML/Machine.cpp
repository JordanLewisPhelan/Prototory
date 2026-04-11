#include "Machine.h"

Machine::Machine(
    const MachineDefinition* t_definition,
    sf::Vector2i             t_gridPosition,
    sf::Vector2f             t_worldOffset)
    : m_definition(t_definition)
    , m_gridPosition(t_gridPosition)
    , m_worldOffset(t_worldOffset)
    , m_inputInventory(t_definition->m_inputSlots > 0 ? t_definition->m_inputSlots : 1)
    , m_outputInventory(t_definition->m_outputSlots > 0 ? t_definition->m_outputSlots : 1)
    , m_tickTimer(0.f)
    , m_idle(false)
{
}

Inventory& Machine::getOutputInventory()
{
    return m_outputInventory;
}

Inventory& Machine::getInputInventory()
{
    return m_inputInventory;
}

sf::Vector2i Machine::getGridPosition() const
{
    return m_gridPosition;
}

sf::Vector2f Machine::getWorldPosition() const
{
    return sf::Vector2f(
        m_gridPosition.x * Globals::TILE_SIZE + m_worldOffset.x,
        m_gridPosition.y * Globals::TILE_SIZE + m_worldOffset.y
    );
}

const MachineDefinition& Machine::getDefinition() const
{
    return *m_definition;
}

bool Machine::isIdle() const
{
    return m_idle;
}