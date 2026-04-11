#pragma once

#include "Machine.h"

class TreeChopper : public Machine
{
public:
    TreeChopper(const MachineDefinition* t_definition,
        sf::Vector2i             t_gridPosition,
        sf::Vector2f             t_worldOffset);

    void update(sf::Time t_dt, TileMap& t_tileMap) override;
    void render(sf::RenderWindow& t_window)        override;
    void onNeighbourChanged()                      override;

private:
    // Scans tiles within harvest range and builds candidate list
    void refreshHarvestCandidates(TileMap& t_tileMap);

    // Attempts to harvest from the next available candidate
    void tick(TileMap& t_tileMap);

    // Candidate tile positions within harvest range that are harvestable
    std::vector<sf::Vector2i> m_harvestCandidates;

    // Visual representation
    sf::RectangleShape m_shape;
    sf::RectangleShape m_rangeIndicator;
    bool               m_showRange{ false };
};