#include "Core/Ember.h"

int main(int argc, char* argv[])
{
	ember::WindowProperties properties("App", 1000, 800);
	ember::WindowsWindow window(&properties);

	ember::Renderer2D renderer(&window);
	ember::EventHandler events(&window);

	if (ember::InitializeAssets())
		std::cout << "All Initialized\n";

	ember::SetWindowIcon(window.GetNativeWindow(), "res/Dungeon_Tileset.png");
	events.ResizeWin();
	ember::Rect position = { 0, 0, 200, 200 };
	ember::ScaleableObject panel(position, &properties.width, &properties.height);

	while (window.IsRunning()) {  
		events.Update();

		ember::Color background = { 67, 120, 220 };
		renderer.Clear(background);

		renderer.Rectangle(position, { 255, 0, 0, 255 });

		position = panel.UpdatePosition();

		renderer.Show();
	}

	return 0;
}