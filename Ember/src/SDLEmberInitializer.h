#ifndef SDL_EMBER_INIT_H
#define SDL_EMBER_INIT_H

#include <string>
#include <SDL.h>

namespace Ember
{
#ifdef _WIN32
	//struct that holds all data needed for creating a window
	struct SDLProperties
	{
		std::string WindowTitle;
		SDL_Window* Window;
		SDL_Renderer* Renderer;
		SDL_Event Event;
		SDLProperties(); //defualt constructor
		SDLProperties(const std::string& Title);
	};
	//class that creates window
	class EmberScreen
	{
	public:
		EmberScreen(SDLProperties& Properties);
		~EmberScreen();
		bool IsRunning;
	private:
		SDLProperties* Properties;
	};
#endif // _WIN32
}

#endif // !SDL_EMBER_INIT_H

