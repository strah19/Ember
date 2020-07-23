#include <Ember.h>
#include <LinkedList.h>

Ember::SDLProperties properties("Ember", 1700, 900, false);
Ember::EmberScreen mainScreen(&properties);

Ember::TileMap tileMap(&mainScreen, 10, 10, 20, 20, 32, 32);

Ember::SpriteSheet sheet(&mainScreen, "res/Dungeon_Tileset.png", 10, 10);

int main(int argc, char* argv[])
{
	Ember::CheckVersion();
	mainScreen.AddEventFlags(Ember::EventFlags::resize);
	mainScreen.AddEventFlags(Ember::EventFlags::escape);
	mainScreen.m_IsRunning = Ember::initialize_all_asset_loaders();

	while (mainScreen.m_IsRunning)
	{
		while (SDL_PollEvent(&properties.m_Event))
			mainScreen.m_IsRunning = mainScreen.IterateEvents();
		mainScreen.cursor.update();

		SDL_SetRenderDrawColor(properties.m_Renderer, 10, 100, 200, 255);
		SDL_RenderClear(properties.m_Renderer);

		tileMap.render_border(255, 255, 255);

		tileMap.RenderSheet(sheet, (10 + (20 * 32)), 10);
		tileMap.UseSheet(sheet);

		SDL_RenderPresent(properties.m_Renderer);
	}

	return 0;
}
