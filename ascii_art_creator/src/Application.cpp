#include "Application.h"

/// <summary>
/// Application constructor.
/// </summary>
Application::Application() : m_window{ sf::VideoMode{ Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 32 }, "ASCII Art Creator", sf::Style::Default }
{
	// Load font
	m_font.loadFromMemory(Globals::DEFAULT_FONT, (size_t)75864 * sizeof(uint8_t));
}

/// <summary>
/// Application destructor.
/// </summary>
Application::~Application()
{

}

/// <summary>
/// Run.
/// </summary>
void Application::run()
{
	sf::Clock f_clock;
	sf::Time f_timeSinceLastUpdate = sf::Time::Zero;
	sf::Time f_timePerFrame = sf::seconds(1.f / 60.0f);

	while (m_window.isOpen() && !m_exitGame)
	{
		f_timeSinceLastUpdate += f_clock.restart();

		while (f_timeSinceLastUpdate > f_timePerFrame)
		{
			f_timeSinceLastUpdate -= f_timePerFrame;
			processEvents();
			update();
			draw();
		}		
	}
}

/// <summary>
/// Process events.
/// </summary>
void Application::processEvents()
{
	sf::Event f_event;

	while (m_window.pollEvent(f_event))
	{
		if (sf::Event::Closed == f_event.type)
		{
			m_window.close();
		}

		if (sf::Event::KeyPressed == f_event.type)
		{
			if (sf::Keyboard::Escape == f_event.key.code)
			{
				m_exitGame = true;
			}
		}
	}
}

/// <summary>
/// Update.
/// </summary>
void Application::update()
{
	
}

/// <summary>
/// Draw.
/// </summary>
void Application::draw()
{
	m_window.clear(sf::Color::Black);

	drawString(16, Globals::SCREEN_HEIGHT - 30, "ASCII ART CREATOR");

	m_window.display();
}

/// <summary>
/// Draws a string to a given position.
/// </summary>
/// <param name="t_x">The X position.</param>
/// <param name="t_y">The Y position.</param>
/// <param name="t_string">The string to draw.</param>
/// <param name="t_colour">The colour of the string.</param>
/// <param name="t_size">The character size (default value of 16).</param>
void Application::drawString(int t_x, int t_y, std::string t_string, sf::Color t_colour, int t_size)
{
	sf::Text f_text;

	f_text.setCharacterSize(t_size);
	f_text.setFont(m_font);
	f_text.setString(t_string);
	f_text.setPosition(t_x, t_y);
	f_text.setFillColor(t_colour);

	m_window.draw(f_text);
}