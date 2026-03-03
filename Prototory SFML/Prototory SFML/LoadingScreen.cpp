#include "LoadingScreen.h"
#include "Globals.h"
#include <algorithm>


LoadingScreen::LoadingScreen(sf::RenderWindow& t_window, sf::Font& t_font)
	: m_window(t_window)
	, m_font(t_font)
	, m_currentProgress(0.0f)
{
	/* - Progression Bar Container Details - */
	m_backgroundBar.setSize(sf::Vector2f(300.0f, 150.f));
	m_backgroundBar.setPosition(sf::Vector2f((Globals::SCREEN_WIDTH - 300.0f) / 2.f,
											(Globals::SCREEN_HEIGHT - 60.f) / 2.f));
	m_backgroundBar.setFillColor(sf::Color(50.f, 30.f, 40.f));
	m_backgroundBar.setOutlineThickness(2.f);
	m_backgroundBar.setOutlineColor(sf::Color::White);

	/* - Progression Bar Details | Will fill left to right - */
	float l_padding = 10.f;

	m_progressBar.setSize(sf::Vector2f(20.f, 60.f - (l_padding * 2)));
	m_progressBar.setPosition(sf::Vector2f(
		m_backgroundBar.getPosition().x + l_padding,
		m_backgroundBar.getPosition().y + l_padding));

	m_progressBar.setFillColor(sf::Color::Cyan);
	m_progressBar.setOutlineThickness(2.f);
	m_progressBar.setOutlineColor(sf::Color::Black);

	/* - Task we are performing when loading - */
	m_taskText.setFont(m_font);
	m_taskText.setCharacterSize(24u);
	m_taskText.setFillColor(sf::Color::White);
	m_taskText.setString("Initializing...");

	sf::FloatRect l_taskBounds = m_taskText.getLocalBounds();

	m_taskText.setPosition(sf::Vector2f(
		(Globals::SCREEN_WIDTH - l_taskBounds.size.x) / 2.f,
		m_backgroundBar.getPosition().y - 60.f));

	m_percentText.setFont(m_font);
	m_percentText.setCharacterSize(20u);
	m_percentText.setFillColor(sf::Color::White);
	m_percentText.setString("0%");

	sf::FloatRect l_percentBounds = m_percentText.getLocalBounds();

	m_percentText.setPosition(sf::Vector2f(
		(Globals::SCREEN_WIDTH - l_percentBounds.size.x) / 2.f,
		m_backgroundBar.getPosition().y + 15.f));
}

void LoadingScreen::setTask(const std::string t_taskText)
{
	m_taskText.setString(t_taskText);

	// Recenter text
	sf::FloatRect l_taskBounds = m_taskText.getLocalBounds();

	m_taskText.setPosition(sf::Vector2f(
		(Globals::SCREEN_WIDTH - l_taskBounds.size.x) / 2.f,
		m_taskText.getPosition().y
	));

	std::string l_debug = m_taskText.getString();

	std::cout << "Task: " << l_debug << "\n\n";
}

void LoadingScreen::setProgress(float t_progress)
{
	// Clamp value between 0 and 1
	m_currentProgress = std::clamp(t_progress, 0.f, 1.f);

	updateProgressBar();

	int l_percent = static_cast<int>(m_currentProgress * 100.f);
	m_percentText.setString(std::to_string(l_percent) + "%");

	sf::FloatRect l_percentBounds = m_percentText.getLocalBounds();

	m_percentText.setPosition(sf::Vector2f(
		(Globals::SCREEN_WIDTH - l_percentBounds.size.x) / 2.f,
		m_percentText.getPosition().y
	));
}

void LoadingScreen::updateProgressBar()
{
	float l_maxWidth = m_backgroundBar.getSize().x - 20.f;
	float l_currentWidth = l_maxWidth * m_currentProgress;

	m_progressBar.setSize(sf::Vector2f(l_currentWidth, m_backgroundBar.getSize().y - 20.f));
}


void LoadingScreen::render()
{
	m_window.clear(sf::Color(20.f, 30.f, 50.f));

	m_window.draw(m_backgroundBar);
	m_window.draw(m_progressBar);
	m_window.draw(m_taskText);
	m_window.draw(m_percentText);

	m_window.display();
}