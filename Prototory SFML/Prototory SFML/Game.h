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

	const sf::Color ULTRAMARINE{ 5, 55,242,255 }; // const colour

	void processEvents();
	void checkKeyboardState();
	void update(sf::Time t_deltaTime);
	void render();
	
	void setupTexts();
	void setupSprites();
	void setupAudio();

	
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_jerseyFont;// font used by message

	SceneManager m_sceneManager;

};

