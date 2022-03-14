#ifndef GLOBALS_H
#define GLOBALS_H

#include <cstdint>

constexpr auto ONE_DEGREE = 0.0174533;
constexpr auto SIX_DEGREES = 0.10472;
constexpr auto THREESIXTY_DEGREES = 6.28319;

constexpr auto SHIP_THRUST = 0.5;

enum class ShapeType
{
	PLAYER,
	SQUARE,
	CROSS,
	STAR,
	TEST
};

class Globals
{
public:
	static const int SCREEN_WIDTH = 1920;
	static const int SCREEN_HEIGHT = 1080;

	static const int WORLD_WIDTH = 320;
	static const int WORLD_HEIGHT = 180;

	static const uint8_t DEFAULT_FONT[];
};

#endif // !GLOBALS_H
