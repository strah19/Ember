#include "Core/Ember.h"

int main(int argc, char* argv[])
{
	ember::WindowProperties properties("App", 1000, 800);
	ember::WindowsWindow window(&properties);

	ember::Renderer2D renderer(&window);
	ember::EventHandler events(&window);
	ember::Camera camera;

	if (ember::InitializeAssets())
		std::cout << "All Initialized\n";

	ember::SetWindowIcon(window.GetNativeWindow(), "res/Dungeon_Tileset.png");
	events.ResizeWin();
	ember::Rect position = { 0, 0, 100, 100 };
	ember::ScaleableObject panel(position, &properties.width, &properties.height);
	ember::Input in(&events);

	ember::Font f(&renderer, "res/Roboto-Medium.ttf", "SomeText", 32, { 0, 0, 0 }, 400, 0);
	f.UnlockFont();

	ember::Entity entity;
	entity.AddComponent<ember::PositionComponent>(0, 0, 100, 100);
	entity.AddComponent<ember::SpriteComponent>("res/Dungeon_Tileset.png", &renderer);

	while (window.IsRunning()) {  
		events.Update();

		ember::Color background = { 67, 120, 220 };
		renderer.Clear(background);

		//renderer.Rectangle(position, { 255, 0, 0, 255 });

		camera.Pan(in, 1);

		int x, y;
		camera.WorldToScreen(0, 0, x, y);
		//position.pos = { x, y };

		entity.GetComponent<ember::PositionComponent>().ChangePosition((float) x, (float) y);

		entity.Render();

		//f.Render();
		renderer.Show();
	}

	ember::AssetCleanUp();

	return 0;
}