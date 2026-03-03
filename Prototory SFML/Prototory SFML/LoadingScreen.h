#pragma once

#include <SFML/Graphics.hpp>
#include <string>


class LoadingScreen
{
public:
	LoadingScreen(sf::RenderWindow& t_window, sf::Font& t_font);

	// Updates to list the current task being worked on
	void setTask(const std::string t_taskText);

	// Percentage Progress of how far we are loaded
	void setProgress(float t_progress);

	void render();

private:
	sf::RenderWindow& m_window;
	sf::Font& m_font;

	// visual components
	sf::RectangleShape m_backgroundBar; 
	sf::RectangleShape m_progressBar;
	sf::Text m_taskText{ m_font };
	sf::Text m_percentText{ m_font };

	float m_currentProgress;

	void updateProgressBar();
};