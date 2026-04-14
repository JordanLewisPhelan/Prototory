#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class LoadingScreen
{
public:
	LoadingScreen(sf::RenderWindow& t_window, sf::Font& t_font);

	
	void setTask(const std::string t_taskText);

	
	void setProgress(float t_progress);

	void render();

private:
	sf::RenderWindow& m_window;
	sf::Font& m_font;

	
	sf::RectangleShape m_backgroundBar; 
	sf::RectangleShape m_progressBar;
	sf::Text m_taskText{ m_font };
	sf::Text m_percentText{ m_font };

	float m_currentProgress;

	void updateProgressBar();
};