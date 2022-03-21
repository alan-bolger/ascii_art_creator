#include "GUI.h"

/// <summary>
/// GUI constructor.
/// </summary>
/// <param name="t_window">A render window.</param>
GUI::GUI(sf::RenderWindow &window) : window(window)
{
	// Load GUI font
	guiFont.loadFromFile("assets/pixellari.ttf");

	// Add items to menu bar
	menuBar.push_back(Button("File", 0, 0, 80, 25, 16, guiFont));
	menuBar.push_back(Button("Edit", 80, 0, 80, 25, 16, guiFont));
	menuBar.push_back(Button("About", 160, 0, 80, 25, 16, guiFont));
}

/// <summary>
/// GUI destructor.
/// </summary>
GUI::~GUI()
{

}

/// <summary>
/// Update.
/// </summary>
void GUI::update()
{
	// Update menu bar
	for (int i = 0; i < menuBar.size(); ++i)
	{
		menuBar[i].update(window);
	}
}

/// <summary>
/// Draw.
/// </summary>
void GUI::draw()
{
	window.setView(window.getDefaultView());

	// Draw menu bar
	for (int i = 0; i < menuBar.size(); ++i)
	{
		window.draw(menuBar[i].button);
		window.draw(menuBar[i].text);
	}
}
