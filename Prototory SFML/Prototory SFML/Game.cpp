#include "Game.h"
#include <iostream>

Game::Game() :
	m_window{ sf::VideoMode{ sf::Vector2u{Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT}, 32U }, "Prototory" },
	m_sceneManager(m_window)
{
	setupTexts(); 
	setupSprites(); 
	setupAudio(); 
}

Game::~Game()
{
}

void Game::run()
{	
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); 
	while (m_window.isOpen())
	{
		processEvents(); 
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); 
			update(timePerFrame); 
		}
		render(); 
	}
}

void Game::processEvents()
{
	
	while (const std::optional newEvent = m_window.pollEvent())
	{
		if ( newEvent->is<sf::Event::Closed>()) 
		{
			
		}
		m_sceneManager.HandleEvent(newEvent, m_window);
	}
}

void Game::checkKeyboardState()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		
	}
}

void Game::update(sf::Time t_deltaTime)
{
	checkKeyboardState();

	m_sceneManager.Update(t_deltaTime);

	if (m_sceneManager.isQuitRequested())
	{
		m_window.close();
	}
}

void Game::render()
{
	m_window.clear(ULTRAMARINE);
	
	m_sceneManager.Render(m_window);

	m_window.display();
}

void Game::setupTexts()
{
	if (!m_jerseyFont.openFromFile("ASSETS\\FONTS\\Jersey20-Regular.ttf"))
	{
		std::cout << "problem loading arial black font" << std::endl;
	}

}

void Game::setupSprites()
{
}

void Game::setupAudio()
{
}
