#include "MainMenu.h"

MainMenu::MainMenu() : m_pendingAction(SceneActions::None)
{
	if (!m_menuFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "MainMenu: font has not been loaded. \n";
	}

	
	m_titleText.setString("Main Menu");
	m_titleText.setCharacterSize(48);
	m_titleText.setFillColor(sf::Color::White);
	m_titleText.setPosition(sf::Vector2f(150.f, 100.f));
	m_titleText.setStyle(sf::Text::Bold | sf::Text::Underlined);

	GenerateButtons();
}

void MainMenu::HandleEvent(const std::optional<sf::Event>& t_event, sf::RenderWindow& t_window)
{
	if (!t_event.has_value()) return;

	sf::Vector2i mousePosI = sf::Mouse::getPosition(t_window);
	sf::Vector2f mousePos(static_cast<float>(mousePosI.x),
		static_cast<float>(mousePosI.y));

	for (auto& button : m_sceneButtons)
	{
		button.HandleEvent(*t_event, mousePos);
	}
}

void MainMenu::Update(sf::Time t_dt)
{
	
	
	
}

void MainMenu::Render(sf::RenderWindow& t_window)
{
	t_window.draw(m_titleText);

	for (auto& button : m_sceneButtons)
	{
		button.Render(t_window);
	}
}

SceneActions MainMenu::getRequestedAction()
{
	SceneActions action = m_pendingAction;
	m_pendingAction = SceneActions::None; 
	return action;
}

void MainMenu::GenerateButtons()
{
	m_sceneButtons.emplace_back(
		sf::Vector2f(100.f, 250.f),
		sf::Vector2f(200.f, 50.f),
		"START GAME",
		m_menuFont
	);

	m_sceneButtons.back().setCallback([this]()
		{
			m_pendingAction = SceneActions::SwitchToGameplay;
			std::cout << "Gameplay begins.. \n";
		});

	m_sceneButtons.emplace_back(
		sf::Vector2f(100.f, 320.f),
		sf::Vector2f(200.f, 50.f),
		"SETTINGS",
		m_menuFont
	);

	m_sceneButtons.back().setCallback([this]()
		{
			
			std::cout << "Settings clicked!\n";
		});

	m_sceneButtons.emplace_back(
		sf::Vector2f(100.f, 390.f),
		sf::Vector2f(200.f, 50.f),
		"QUIT",
		m_menuFont
	);

	m_sceneButtons.back().setCallback([this]()
		{
			m_pendingAction = SceneActions::QuitGame;
			std::cout << "Quit clicked!\n";
		});
}
