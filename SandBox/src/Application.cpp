#include <Ember.h>
#include <SDL.h>

Ember::SDLProperties Properties("Ember Application");
Ember::EmberScreen Screen(Properties);

void ExampleOne()
{
	SDL_Renderer* ren = Properties.Renderer;
	Ember::rounded_rectangle_fill(ren, 550, 300, 750, 500, 15, 255, 0, 0, 255);
}

int main(int argc, char* argv[])
{
	Ember::initialize_events();
	Ember::add_event_flags(Ember::EventFlags::ESCAPE_CLOSE);
	Ember::add_event_flags(Ember::EventFlags::RESIZEABLE);

	Ember::initialize_font();
	Ember::initialize_img();
	Ember::initialize_mixer();

	while (Screen.IsRunning){
		while(SDL_PollEvent(&Properties.Event))
			Screen.IsRunning = Ember::iterate_events(Properties);

		SDL_SetRenderDrawColor(Properties.Renderer, 100, 200, 190, 255);
		SDL_RenderClear(Properties.Renderer);

		ExampleOne();

		SDL_RenderPresent(Properties.Renderer);
	}

	return 0;
}