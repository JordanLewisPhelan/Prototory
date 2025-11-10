#include "Gameplay.h"

Gameplay::Gameplay() 
	: m_pendingAction(SceneActions::None), m_tileMap(128, 128, Globals::TILE_SIZE)
{
	if (!m_gameFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "Gameplay: Font has not been loaded. \n";
	}

	// default text for debugging - will refine later(intended to show key data later)
	m_areaText.setString("Gameplay Screen");
	m_areaText.setCharacterSize(36);
	m_areaText.setFillColor(sf::Color::White);
	m_areaText.setPosition(sf::Vector2f(
		Globals::SCREEN_WIDTH / 2.5f, Globals::SCREEN_HEIGHT / 3.5f
	));
	m_areaText.setStyle(sf::Text::Underlined | sf::Text::Italic);	
}

// Will be used to determine player key presses and click inputs during gameplay
void Gameplay::HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window)
{
	if (!t_event.has_value()) return;

	if (t_event->is<sf::Event::KeyPressed>())
	{
		const auto* keyPress = t_event->getIf<sf::Event::KeyPressed>();
		if (keyPress && keyPress->code == sf::Keyboard::Key::Escape)
		{
			m_pendingAction = SceneActions::SwitchToMenu;
		}
	}
}

void Gameplay::Update(sf::Time t_dt)
{
	/*--
	ToDo: Separate Later on into Paused & Unpaused will help clarify operations
	--*/

	// Just general gameplay stuff for during gameplay, nothing to add just yet.
}

void Gameplay::Render(sf::RenderWindow& t_window)
{
	m_tileMap.render(t_window);

	t_window.draw(m_areaText);
}

SceneActions Gameplay::getRequestedAction()
{
	SceneActions action = m_pendingAction;
	m_pendingAction = SceneActions::None;
	return action;
}
