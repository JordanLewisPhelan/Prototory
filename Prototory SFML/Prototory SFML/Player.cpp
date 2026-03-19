#include "Player.h"

Player::Player(const sf::Vector2f& t_startPos, const sf::Vector2f& t_size)
	: m_position(t_startPos)
	, m_playerSize(t_size)
	, m_velocity(0.f, 0.f)
	, m_walkSpeed(250.f)
    , m_acceleration(750.f)
    , m_deceleration(600.f)
{
	// Sprite Creation
	m_playerSprite.setSize(m_playerSize);
	m_playerSprite.setFillColor(sf::Color::Red);
	m_playerSprite.setOrigin(sf::Vector2f(m_playerSize.x / 2.f, m_playerSize.y / 2.f));
	m_playerSprite.setPosition(m_position);

	std::cout << "Player created at: (" << m_position.x << ", " << m_position.y << ")\n";
}


void Player::handleInput(sf::Time t_dt)
{
	sf::Vector2f l_direction = getInputDirection();

    // Accelerator force
    if (l_direction.x != 0.f || l_direction.y != 0)
    {
        sf::Vector2f l_desiredVelocity = l_direction * m_walkSpeed;

        // Aimed velocity difference to current velocity
        sf::Vector2f l_velocityDelta = l_desiredVelocity - m_velocity;

        // Prevents jumping to the intended velocity immediately, enforces the acceleration
        float l_adjustmentCap = m_acceleration * t_dt.asSeconds();

        // Comparison between the adjustment we allow and this value - the magnitude
        float l_adjustmentMagnitude = std::sqrt(l_velocityDelta.x * l_velocityDelta.x +
                                                l_velocityDelta.y * l_velocityDelta.y);

        if (l_adjustmentCap < l_adjustmentMagnitude)
        {
            l_velocityDelta = (l_velocityDelta / l_adjustmentMagnitude) * l_adjustmentCap;

            m_velocity += l_velocityDelta;
        }
    }

    // Decelerator force
    else
    {
        float l_currentSpeed = std::sqrt(m_velocity.x * m_velocity.x +
                                        m_velocity.y * m_velocity.y);

        if (l_currentSpeed > 0.f)
        {
            float l_decelerationVelocity = m_deceleration * t_dt.asSeconds();

            // Close enough to stopped - stop fully
            if (l_decelerationVelocity >= l_currentSpeed)
            {
                m_velocity = sf::Vector2f(0.f, 0.f);
            }

            // Decelerate gradually
            else
            {
                // Decelerate based on direction, this computation prevents negative values accelerating - e.g. going left and up then letting go of inputs
                sf::Vector2f l_decelDir = m_velocity / l_currentSpeed;
                m_velocity -= l_decelDir * l_decelerationVelocity;
            }
        }
    }

}


void Player::update(sf::Time t_dt, const TileMap& t_tileMap)
{
    handleInput(t_dt);

    checkElevationMovement(t_tileMap, t_dt);

    m_position += m_velocity * t_dt.asSeconds();

    if (m_blockTimer > 0.f)
    {
        m_blockTimer -= t_dt.asSeconds();
        m_playerSprite.setFillColor(BLOCKED_COLOUR);
    }
    else
    {
        m_playerSprite.setFillColor(sf::Color::Red);
    }

	constrainToWorldBounds(t_tileMap);

	m_playerSprite.setPosition(m_position);

}


void Player::render(sf::RenderWindow& t_window)
{
	t_window.draw(m_playerSprite);
}


sf::Vector2i Player::getGridPosition(const TileMap& t_tileMap) const
{
    return t_tileMap.worldToGrid(m_position);
}

sf::Vector2f Player::getInputDirection() const
{
    sf::Vector2f l_direction(0.f, 0.f);


    // WASD + Arrow Keys
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
    {
        l_direction.y -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
    {
        l_direction.y += 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
    {
        l_direction.x -= 1.f;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
    {
        l_direction.x += 1.f;
    }

    // Normalize for diagonal movement
    float length = std::sqrt(l_direction.x * l_direction.x + l_direction.y * l_direction.y);
    if (length != 0.f)
    {
        l_direction /= length;
    }

    return l_direction;
}

void Player::constrainToWorldBounds(const TileMap& t_tileMap)
{
    float l_worldWidth = t_tileMap.getWidth() * Globals::TILE_SIZE;
    float l_worldHeight = t_tileMap.getHeight() * Globals::TILE_SIZE;

    // accounts for sprite size
    float l_halfWidth = m_playerSize.x / 2.f;
    float l_halfHeight = m_playerSize.y / 2.f;

    // Clamp to world edges
    if (m_position.x - l_halfWidth < 0.f)
        m_position.x = l_halfWidth;
    if (m_position.x + l_halfWidth > l_worldWidth)
        m_position.x = l_worldWidth - l_halfWidth;
    if (m_position.y - l_halfHeight < 0.f)
        m_position.y = l_halfHeight;
    if (m_position.y + l_halfHeight > l_worldHeight)
        m_position.y = l_worldHeight - l_halfHeight;
}

void Player::checkElevationMovement(const TileMap& t_tileMap, sf::Time t_dt)
{
    sf::Vector2i l_currentGridPos = t_tileMap.worldToGrid(m_position);
    int l_currentElevation = t_tileMap.getElevationAt(l_currentGridPos.x, l_currentGridPos.y);

    // X axis checks
    sf::Vector2f l_targetX = m_position + sf::Vector2f(m_velocity.x * t_dt.asSeconds(), 0.f);
    sf::Vector2i l_targetGridX = t_tileMap.worldToGrid(l_targetX);
    int l_elevationX = t_tileMap.getElevationAt(l_targetGridX.x, l_targetGridX.y);

    if (std::abs(l_elevationX - l_currentElevation) > MAX_ELEVATION_DELTA)
    {
        m_velocity.x = 0.f;
        m_blockTimer = BLOCKED_FLASH_DURATION;
    }

    // Y axis checks
    sf::Vector2f l_targetY = m_position + sf::Vector2f(0.f, m_velocity.y * t_dt.asSeconds());
    sf::Vector2i l_targetGridY = t_tileMap.worldToGrid(l_targetY);
    int l_elevationY = t_tileMap.getElevationAt(l_targetGridY.x, l_targetGridY.y);

    if (std::abs(l_elevationY - l_currentElevation) > MAX_ELEVATION_DELTA)
    {
        m_velocity.y = 0.f;
        m_blockTimer = BLOCKED_FLASH_DURATION;
    }
}
