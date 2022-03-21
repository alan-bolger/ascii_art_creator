#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <functional>

static const sf::Color BUTTON_HOVER{ 200, 200, 200 };
static const sf::Color BUTTON_IDLE{ 100, 100, 100 };

class GUI
{
public:
	GUI(sf::RenderWindow &window);
	~GUI();
	void update();
	void draw();

private:
	struct Button
	{
		Button(const std::string &label, int x, int y, int w, int h, int textSize, sf::Font &font)
		{
			button.setPosition(x, y);
			button.setSize(sf::Vector2f(w, h));

			text.setFont(font);
			text.setCharacterSize(textSize);
			text.setString(label);
		};

		sf::RectangleShape button;
		sf::Text text;
		std::function<void(void)> function;

		// This is the function called when the button is clicked
		void setFunction(std::function<void()> function)
		{
			this->function = function;
		}

		// Check for button triggers
		void update(sf::RenderWindow &window)
		{
			if (button.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				button.setFillColor(BUTTON_HOVER);

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					function();
				}
			}
			else
			{
				button.setFillColor(BUTTON_IDLE);
			}

			// Sets the position of the text to be centered on the button
			text.setPosition(static_cast<int>(button.getPosition().x + (button.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2)),
				static_cast<int>(button.getPosition().y + (button.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2) - 3));
		}

		// Draw the button
		void draw(sf::RenderWindow &window)
		{
			window.draw(button);
			window.draw(text);
		}
	};

	sf::RenderWindow &window;
	sf::Font guiFont;
	std::vector<Button> menuBar;
};

#endif // !GUI_H