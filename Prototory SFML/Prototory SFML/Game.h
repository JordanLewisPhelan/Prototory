#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Globals.h"
#include "SceneManager.h"

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	const sf::Color ULTRAMARINE{ 5, 55,242,255 }; 

	void processEvents();
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupTexts();
	void setupSprites();
	void setupAudio();

	
	sf::RenderWindow m_window; 
	sf::Font m_jerseyFont;

	SceneManager m_sceneManager;

};

