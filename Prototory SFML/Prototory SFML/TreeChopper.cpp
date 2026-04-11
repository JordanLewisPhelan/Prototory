#include "TreeChopper.h"
#include "TileMap.h"
#include "Globals.h"

TreeChopper::TreeChopper(
    const MachineDefinition* t_definition,
    sf::Vector2i             t_gridPosition,
    sf::Vector2f             t_worldOffset)
    : Machine(t_definition, t_gridPosition, t_worldOffset)
{
    // Main body shape - smaller than a tile, offset to left edge per placement
    float l_size = Globals::TILE_SIZE * 0.5f;
    m_shape.setSize({ l_size, l_size });
    m_shape.setFillColor(sf::Color(139, 90, 43));    // wood brown
    m_shape.setOutlineColor(sf::Color(80, 50, 20));
    m_shape.setOutlineThickness(1.5f);
    m_shape.setOrigin(sf::Vector2f(l_size / 2.f, l_size / 2.f));

    // Range indicator - drawn at low opacity around harvest area
    float l_rangeSize = Globals::TILE_SIZE
        * (1.f + 2.f * t_definition->m_harvestRange);
    m_rangeIndicator.setSize({ l_rangeSize, l_rangeSize });
    m_rangeIndicator.setFillColor(sf::Color(100, 200, 100, 40));
    m_rangeIndicator.setOutlineColor(sf::Color(100, 200, 100, 120));
    m_rangeIndicator.setOutlineThickness(1.f);
    m_rangeIndicator.setOrigin(sf::Vector2f(l_rangeSize / 2.f, l_rangeSize / 2.f));
}

void TreeChopper::update(sf::Time t_dt, TileMap& t_tileMap)
{
    // If we have no candidates, try to find some
    if (m_harvestCandidates.empty())
    {
        refreshHarvestCandidates(t_tileMap);

        if (m_harvestCandidates.empty())
        {
            m_idle = true;
            return;
        }
    }

    m_idle = false;

    // Advance tick timer
    m_tickTimer += t_dt.asSeconds();

    if (m_tickTimer >= m_definition->m_tickRate)
    {
        m_tickTimer = 0.f;
        tick(t_tileMap);
    }
}

void TreeChopper::tick(TileMap& t_tileMap)
{
    if (m_outputInventory.isFull())
        return;

    // Work through candidates until we successfully harvest one
    for (int l_i = 0; l_i < static_cast<int>(m_harvestCandidates.size()); )
    {
        sf::Vector2i l_pos = m_harvestCandidates[l_i];
        TileResource& l_resource = t_tileMap.getTileResourceAt(l_pos.x, l_pos.y);

        if (!l_resource.isHarvestable())
        {
            // Tile exhausted - remove from candidates
            m_harvestCandidates.erase(m_harvestCandidates.begin() + l_i);
            continue;
        }

        // Harvest
        int l_amount = std::min(m_definition->m_harvestAmount,
            l_resource.m_currentQuantity);

        l_resource.m_currentQuantity -= l_amount;

        m_outputInventory.addResources(
            l_resource.m_resourceID,
            l_amount,
            m_definition->m_outputSlots
        );

        return; // One harvest per tick
    }

    // If we reach here all candidates are exhausted
    m_harvestCandidates.clear();
    m_idle = true;
}

void TreeChopper::refreshHarvestCandidates(TileMap& t_tileMap)
{
    m_harvestCandidates.clear();

    int l_range = m_definition->m_harvestRange;

    for (int l_dy = -l_range; l_dy <= l_range; l_dy++)
    {
        for (int l_dx = -l_range; l_dx <= l_range; l_dx++)
        {
            sf::Vector2i l_pos = m_gridPosition + sf::Vector2i(l_dx, l_dy);

            // Skip the tile the machine itself sits on
            if (l_pos == m_gridPosition)
                continue;

            // Bounds check
            if (l_pos.x < 0 || l_pos.x >= Globals::WORLD_WIDTH ||
                l_pos.y < 0 || l_pos.y >= Globals::WORLD_HEIGHT)
                continue;

            const TileResource& l_resource =
                t_tileMap.getTileResourceAt(l_pos.x, l_pos.y);

            if (l_resource.isHarvestable() &&
                l_resource.m_interactionType == m_definition->m_harvestType)
            {
                m_harvestCandidates.push_back(l_pos);
            }
        }
    }
}

void TreeChopper::onNeighbourChanged()
{
    // Conveyor linking will be handled here later
    // For now nothing to do
}

void TreeChopper::render(sf::RenderWindow& t_window)
{
    sf::Vector2f l_worldPos = getWorldPosition();
    sf::Vector2f l_pos;

    // Draw range indicator first so it sits beneath the machine shape
    if (!m_idle)
    {
        l_pos = sf::Vector2f(l_worldPos.x + Globals::TILE_SIZE / 2.f,
                             l_worldPos.y + Globals::TILE_SIZE / 2.f);

        m_rangeIndicator.setPosition(l_pos);
        t_window.draw(m_rangeIndicator);
    }

    l_pos = sf::Vector2f(l_worldPos.x + Globals::TILE_SIZE / 2.f + m_worldOffset.x,
                        l_worldPos.y + Globals::TILE_SIZE / 2.f + m_worldOffset.y);

    m_shape.setPosition(l_pos);

    // Tint grey when idle to give visual feedback
    m_shape.setFillColor(m_idle
        ? sf::Color(120, 120, 120)
        : sf::Color(139, 90, 43));

    t_window.draw(m_shape);
}