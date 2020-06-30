#include <Ember.h>
#include <SDL.h>

Ember::SDLProperties Properties("Ember Application");
Ember::EmberScreen Screen(Properties);

void ExampleOne()
{
	SDL_Renderer* ren = Properties.Renderer;
	Ember::pixel(ren, 0, 0, 255, 0, 0, 255);
	Ember::vertical_line(ren, 0, 100, 10, 255, 0, 0, 255);
	Ember::horizontal_line(ren, 0, 100, 10, 255, 0, 0, 255);
	Ember::draw_circle(ren, 50, 50, 30, 255, 0, 0, 255);
	Ember::fill_circle(ren, 100, 100, 30, 255, 0, 0, 255);
	Ember::rectangle_border(ren, 200, 10, 50, 50, 255, 0, 0, 255);
	Ember::rectangle_fill(ren, 10, 200, 50, 50, 255, 0, 0, 255);
	Ember::arcs(ren, 300, 300, 20, 30, 1, 0, 0, 0, 255, 0, 0, 255);
	Ember::arcs(ren, 400, 300, 20, 30, 0, 1, 0, 0, 255, 0, 0, 255);
	Ember::arcs(ren, 500, 300, 20, 30, 0, 0, 1, 0, 255, 0, 0, 255);
	Ember::arcs(ren, 600, 300, 20, 30, 0, 0, 0, 1, 255, 0, 0, 255);
	Ember::rounded_rectangle_border(ren, 30, 300, 200, 500, 15, 255, 0, 0, 255);
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