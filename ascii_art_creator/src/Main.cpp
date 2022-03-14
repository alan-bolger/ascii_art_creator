#include "Application.h"

/// <summary>
/// ASCII Art Creator.
/// Creates ASCII art using images or videos.
/// </summary>
/// <returns>1 for successful exit.</returns>
int main()
{
	Application &f_app = Application();
	f_app.run();

	return 1;
}