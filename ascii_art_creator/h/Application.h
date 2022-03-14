#ifndef APPLICATION_H
#define APPLICATION_H

#include "Globals.h"

#include <SFML/Graphics.hpp>

class Application
{
public:
	Application();
	~Application();
	void run();

private:
	sf::RenderWindow m_window;
	bool m_exitGame{ false };
	sf::Font m_font;

	void processEvents();
	void update();
	void draw();
	void drawString(int t_x, int t_y, std::string t_string, sf::Color t_colour = sf::Color::White, int t_size = 16);
};

#endif // !APPLICATION_H