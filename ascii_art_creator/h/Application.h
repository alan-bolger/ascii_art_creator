#ifndef APPLICATION_H
#define APPLICATION_H

#include "Globals.h"

#include <SFML/Graphics.hpp>
#include <functional>
#include <algorithm>

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
	sf::View m_view;
	sf::Texture m_texture;
	uint8_t *m_original;
	uint8_t *m_greyscale;
	uint8_t *m_oneBit;
	uint8_t *m_quantised;
	uint8_t *m_dithered;
	int m_imageSizeX = 0;
	int m_imageSizeY = 0;
	bool m_panning = false;
	sf::Vector2i m_panningAnchor;
	float m_zoomLevel = 1.0f;

	void floydSteinbergDither(const uint8_t *t_source, uint8_t *t_destination, std::function<sf::Color(const sf::Color)> t_quantiseFunction);
	void processEvents();
	void update();
	void draw();
	void drawString(int t_x, int t_y, std::string t_string, sf::Color t_colour = sf::Color::White, int t_size = 16);
};

#endif // !APPLICATION_H