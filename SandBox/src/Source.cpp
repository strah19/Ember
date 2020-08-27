#include "Core/Ember.h"

int main(int argc, char* argv[])
{
	ember::WindowProperties properties("App", 1000, 800);
	ember::WindowsWindow window(&properties);
	ember::Renderer2D renderer(&window);
	ember::EventHandler events(&window);
	ember::Input input(&events);

	ember::InitializeAssets();

	ember::Entity entity;
	entity.AddComponent<ember::PositionComponent>(0, 0, 200, 200);
	entity.AddComponent<ember::SpriteComponent>("res/Dungeon_Tileset.png", &renderer);
	entity.AddComponent<ember::DebugComponent>(&input);
	
	ember::Camera camera(0.0f, 0.0f);

	float d = 0.0f;
	Uint32 l = 0;

	while (window.IsRunning()) {  
		d = (float)(SDL_GetTicks() - l);
		l = SDL_GetTicks();

		events.Update();
		renderer.Clear({ 67, 120, 220 });

		camera.Pan(input, 1);

		entity.Update();
		entity.Render();

		int screen_x, screen_y;
		camera.WorldToScreen(0, 0, screen_x, screen_y);
		entity.GetComponent<ember::PositionComponent>().ChangePosition(screen_x, screen_y);

		renderer.Show();
	}


	ember::AssetCleanUp();

	return 0;
}