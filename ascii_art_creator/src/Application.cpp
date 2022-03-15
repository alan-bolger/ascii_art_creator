#include "Application.h"

/// <summary>
/// Application constructor.
/// </summary>
Application::Application() : m_window{ sf::VideoMode{ Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT, 32 }, "ASCII Art Creator", sf::Style::Default }
{
	// Load font
	m_font.loadFromMemory(Globals::DEFAULT_FONT, (size_t)75864 * sizeof(uint8_t));

	// Set view
	m_view.setSize(Globals::SCREEN_WIDTH, Globals::SCREEN_HEIGHT);
	m_view.setCenter(Globals::SCREEN_WIDTH / 2, Globals::SCREEN_HEIGHT / 2);

	// Load an image and obtain its dimensions
	sf::Image f_tempImage;
	f_tempImage.loadFromFile("assets/norway.jpg");

	m_imageSizeX = f_tempImage.getSize().x;
	m_imageSizeY = f_tempImage.getSize().y;

	// Create empty pixel arrays for additional image storage
	m_original = new uint8_t[m_imageSizeX * m_imageSizeY * 4];
	m_greyscale = new uint8_t[m_imageSizeX * m_imageSizeY * 4];
	m_dithered = new uint8_t[m_imageSizeX * m_imageSizeY * 4];

	auto f_pixelPtr = f_tempImage.getPixelsPtr(); // Get pointer to beginning of the image's pixel array
	sf::Color f_pixel(0, 0, 0, 0); // Temporarily stores pixel attributes

	// Copy temp image into memory
	for (int i = 0; i < m_imageSizeX * m_imageSizeY; ++i)
	{
		m_original[i * 4] = f_pixelPtr[i * 4];
		m_original[i * 4 + 1] = f_pixelPtr[i * 4 + 1];
		m_original[i * 4 + 2] = f_pixelPtr[i * 4 + 2];
		m_original[i * 4 + 3] = f_pixelPtr[i * 4 + 3];
	}

	// Lambda to convert RGB pixels to greyscale pixels
	auto f_rgbToGs = [](sf::Color t_pixel)
	{
		uint8_t f_gs = uint8_t(0.2162f * float(t_pixel.r) + 0.7152f * float(t_pixel.g) + 0.0722f * float(t_pixel.b));
		return sf::Color(f_gs, f_gs, f_gs, 255);
	};

	// Create greyscale image from loaded image
	for (int i = 0; i < m_imageSizeX * m_imageSizeY; ++i)
	{
		f_pixel.r = f_pixelPtr[i * 4];
		f_pixel.g = f_pixelPtr[i * 4 + 1];
		f_pixel.b = f_pixelPtr[i * 4 + 2];
		f_pixel.a = f_pixelPtr[i * 4 + 3];

		sf::Color f_newPixel = f_rgbToGs(f_pixel);

		m_greyscale[i * 4] = uint8_t(f_newPixel.r);
		m_greyscale[i * 4 + 1] = uint8_t(f_newPixel.g);
		m_greyscale[i * 4 + 2] = uint8_t(f_newPixel.b);
		m_greyscale[i * 4 + 3] = uint8_t(f_newPixel.a);
	}

	// Lambda to quantise greyscale to a selection of bits
	auto f_quantiseGsToBits = [](const sf::Color t_pixel)
	{
		constexpr int f_bits = 3;
		constexpr float f_levels = (1 << f_bits) - 1;
		uint8_t f_clamped = uint8_t(std::clamp(std::round(float(t_pixel.r) / 255.0f * f_levels) / f_levels * 255.0f, 0.0f, 255.0f));

		return sf::Color(f_clamped, f_clamped, f_clamped);
	};

	floydSteinbergDither(m_greyscale, m_dithered, f_quantiseGsToBits);

	// Copy image to texture
	m_texture.create(m_imageSizeX, m_imageSizeY);
	m_texture.update(m_dithered);

	// Create render texture for ASCII text
	m_renderedText.create(m_imageSizeX, m_imageSizeY);

	// Pixel processing stuff
	int f_stepsX = m_imageSizeX / 8;
	int f_stepsY = m_imageSizeY / 8;

	for (int f_startX = 0; f_startX < 8; ++f_startX)
	{
		for (int f_startY = 0; f_startY < 8; ++f_startY)
		{

		}
	}


}

/// <summary>
/// Application destructor.
/// </summary>
Application::~Application()
{
	delete[] m_original;
	m_original = nullptr;

	delete[] m_greyscale;
	m_greyscale = nullptr;

	delete[] m_dithered;
	m_dithered = nullptr;
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
/// Floyd-Steinberg dither.
/// </summary>
/// <param name="t_source">The source image.</param>
/// <param name="t_destination">The destination array.</param>
/// <param name="t_quantiseFunction">A lambda that provides quantisation.</param>
void Application::floydSteinbergDither(const uint8_t *t_source, uint8_t *t_destination, std::function<sf::Color(const sf::Color)> t_quantiseFunction)
{
	// Copy original image to destination image
	for (int i = 0; i < m_imageSizeX * m_imageSizeY; ++i)
	{
		t_destination[i * 4] = t_source[i * 4];
		t_destination[i * 4 + 1] = t_source[i * 4 + 1];
		t_destination[i * 4 + 2] = t_source[i * 4 + 2];
		t_destination[i * 4 + 3] = t_source[i * 4 + 3];
	}

	// Apply quantisation
	sf::Vector2i f_pixel;

	for (f_pixel.y = 0; f_pixel.y < m_imageSizeY; ++f_pixel.y)
	{
		for (f_pixel.x = 0; f_pixel.x < m_imageSizeX; ++f_pixel.x)
		{
			sf::Color f_originalPixel;

			f_originalPixel.r = t_destination[(f_pixel.y * m_imageSizeX + f_pixel.x) * 4];
			f_originalPixel.g = t_destination[(f_pixel.y * m_imageSizeX + f_pixel.x) * 4 + 1];
			f_originalPixel.b = t_destination[(f_pixel.y * m_imageSizeX + f_pixel.x) * 4 + 2];
			f_originalPixel.a = 255;

			sf::Color f_quantisedPixel = t_quantiseFunction(f_originalPixel);

			int32_t f_error[3] =
			{
				f_originalPixel.r - f_quantisedPixel.r,
				f_originalPixel.g - f_quantisedPixel.g,
				f_originalPixel.b - f_quantisedPixel.b
			};

			t_destination[(f_pixel.y * m_imageSizeX + f_pixel.x) * 4] = f_quantisedPixel.r;
			t_destination[(f_pixel.y * m_imageSizeX + f_pixel.x) * 4 + 1] = f_quantisedPixel.g;
			t_destination[(f_pixel.y * m_imageSizeX + f_pixel.x) * 4 + 2] = f_quantisedPixel.b;

			auto f_updatePixel = [&](const sf::Vector2i &t_offset, const float t_errorBias)
			{
				sf::Color f_newPixel;
				sf::Vector2i f_neighbour = f_pixel + t_offset;

				// Make sure no out of bound pixels are selected
				if (f_neighbour.x < m_imageSizeX && f_neighbour.x >= 0)
				{
					if (f_neighbour.y < m_imageSizeY && f_neighbour.y >= 0)
					{
						f_newPixel.r = t_destination[(f_neighbour.y * m_imageSizeX + f_neighbour.x) * 4];
						f_newPixel.g = t_destination[(f_neighbour.y * m_imageSizeX + f_neighbour.x) * 4 + 1];
						f_newPixel.b = t_destination[(f_neighbour.y * m_imageSizeX + f_neighbour.x) * 4 + 2];
						f_newPixel.a = 255;

						int32_t f_intErr[3] = { f_newPixel.r, f_newPixel.g, f_newPixel.b };

						f_intErr[0] += int32_t(float(f_error[0]) * t_errorBias);
						f_intErr[1] += int32_t(float(f_error[1]) * t_errorBias);
						f_intErr[2] += int32_t(float(f_error[2]) * t_errorBias);

						t_destination[(f_neighbour.y * m_imageSizeX + f_neighbour.x) * 4] = std::clamp(f_intErr[0], 0, 255);
						t_destination[(f_neighbour.y * m_imageSizeX + f_neighbour.x) * 4 + 1] = std::clamp(f_intErr[1], 0, 255);
						t_destination[(f_neighbour.y * m_imageSizeX + f_neighbour.x) * 4 + 2] = std::clamp(f_intErr[2], 0, 255);
					}
				}
				else
				{
					return;
				}
			};

			f_updatePixel(sf::Vector2i(1, 0), 7.0f / 16.0f);
			f_updatePixel(sf::Vector2i(-1, 1), 3.0f / 16.0f);
			f_updatePixel(sf::Vector2i(0, 1), 5.0f / 16.0f);
			f_updatePixel(sf::Vector2i(1, 1), 1.0f / 16.0f);
		}
	}
}

/// <summary>
/// Convert a square selection of pixels to an ASCII character.
/// </summary>
/// <param name="t_pixelCoord">The top-left corner coordinate of the section.</param>
/// <param name="t_areaSize">The size of the section in pixels (this size is for both width and height).</param>
void Application::pixelsToASCII(sf::Vector2i t_pixelCoord, int t_areaSize)
{
	std::string f_ascii[10] = { " ", ".", ":", "-", "=", "+", "*", "#", "%", "@" };

	int f_totalAvg = 0;

	for (int y = t_pixelCoord.y; y < t_pixelCoord.y + t_areaSize; ++y)
	{
		for (int x = t_pixelCoord.x; x < t_pixelCoord.x + t_areaSize; ++x)
		{
			int f_pixelAvg = 0;

			f_pixelAvg += m_dithered[(t_pixelCoord.y * m_imageSizeX + t_pixelCoord.x) * 4];
			f_pixelAvg += m_dithered[(t_pixelCoord.y * m_imageSizeX + t_pixelCoord.x) * 4 + 1];
			f_pixelAvg += m_dithered[(t_pixelCoord.y * m_imageSizeX + t_pixelCoord.x) * 4 + 2];

			f_totalAvg += f_pixelAvg / 3;
		}
	}

	f_totalAvg = f_totalAvg / (t_areaSize * t_areaSize);
	std::string f_char = f_ascii[f_totalAvg / 25];
	
	drawString(t_pixelCoord.x, t_pixelCoord.y, f_char, sf::Color::White, t_areaSize);
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

		if (sf::Event::MouseMoved == f_event.type)
		{
			if (m_panning)
			{
				sf::Vector2f f_position = sf::Vector2f(sf::Mouse::getPosition(m_window) - m_panningAnchor);
				m_view.move(-1.0f * f_position * m_zoomLevel);
				m_panningAnchor = sf::Mouse::getPosition(m_window);
			}
		}

		if (sf::Event::MouseButtonPressed == f_event.type)
		{
			if (f_event.mouseButton.button == sf::Mouse::Right)
			{
				if (m_panning != true)
				{
					m_panning = true;
					m_panningAnchor = sf::Mouse::getPosition(m_window);
				}
			}
		}

		if (sf::Event::MouseButtonReleased == f_event.type)
		{
			if (f_event.mouseButton.button == sf::Mouse::Right)
			{
				m_panning = false;
			}
		}

		if (sf::Event::MouseWheelMoved == f_event.type)
		{
			if (f_event.mouseWheel.delta < 0)
			{
				m_view.zoom(2.0f);
				m_zoomLevel *= 2.0f;
			}
			else
			{
				m_view.zoom(0.5f);
				m_zoomLevel *= 0.5f;
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

	m_window.setView(m_view);

	sf::Sprite m_sprite(m_texture);
	m_window.draw(m_sprite);

	m_window.setView(m_window.getDefaultView());

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

	m_renderedText.draw(f_text);
}