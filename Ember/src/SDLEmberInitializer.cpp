#include "SDLEmberInitializer.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

namespace Ember
{
	SDLProperties::SDLProperties()
		: WindowTitle("EmberScreen"), Window(NULL), Renderer(NULL)
	{
	}

	SDLProperties::SDLProperties(const std::string& Title)
		: WindowTitle(Title), Window(NULL), Renderer(NULL)
	{
	}

	EmberScreen::EmberScreen(SDLProperties& Properties)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { //init SDL2
			IsRunning = false;
			std::cout << "Could Not Initialize SDL_INIT" << std::endl;
			SDL_Quit();
		}
		else {
			Properties.Window = SDL_CreateWindow(Properties.WindowTitle.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1000, 1000, SDL_WINDOW_OPENGL); //creates window
			Properties.Renderer = SDL_CreateRenderer(Properties.Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //creates renderer

			if (Properties.Renderer == NULL || Properties.Window == NULL) { //check that renderer has been created properly
				IsRunning = false;
				std::cout << "Could Not Properly Create A Window And Renderer" << std::endl;
				SDL_Quit();
			}
			else {
				IsRunning = true;
				std::cout << "Loaded SDL2 And Created A Window And Renderer" << std::endl;
			}
		}
		this->Properties = &Properties;
	}

	EmberScreen::~EmberScreen()
	{
		//clean-up
		SDL_DestroyWindow(Properties->Window);
		SDL_DestroyRenderer(Properties->Renderer);

		std::cout << "Closing And Cleaning Ember/SDL2" << std::endl;

		IMG_Quit();
		TTF_Quit();
		Mix_Quit();
		SDL_Quit();
	}
}