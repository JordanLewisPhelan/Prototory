#pragma once
#include <SFML/Graphics.hpp>
#include "MachineDefinition.h"
#include "Inventory.h"
#include "Globals.h"



class TileMap;

class Machine
{
public:
    Machine(const MachineDefinition* t_definition,
        sf::Vector2i             t_gridPosition,
        sf::Vector2f             t_worldOffset);

    virtual ~Machine() = default;

    virtual void update(sf::Time t_dt, TileMap& t_tileMap) = 0;
    virtual void render(sf::RenderWindow& t_window) = 0;
    virtual void onNeighbourChanged() {}

    Inventory& getOutputInventory();
    Inventory& getInputInventory();
    sf::Vector2i        getGridPosition()  const;
    sf::Vector2f        getWorldPosition() const;
    const MachineDefinition& getDefinition() const;
    bool                isIdle()           const;

protected:
    sf::Vector2i             m_gridPosition;
    sf::Vector2f             m_worldOffset;
    const MachineDefinition* m_definition;
    Inventory                m_inputInventory;
    Inventory                m_outputInventory;
    float                    m_tickTimer{ 0.f };
    bool                     m_idle{ false };
};