#include "Core/Ember.h"

int main(int argc, char* argv[])
{
	ember::WindowProperties properties("App", 1000, 800);
	ember::WindowsWindow window(&properties);
	ember::Renderer2D renderer(&window);
	ember::EventHandler events(&window);
	ember::Input input(&events);

	ember::InitializeAssets();

	ember::GridComponents gc;
	ember::TileMapSerializer ser(gc, "tiles.txt");
	ember::TileMapSerializer ser2(gc, "tiles2.txt");

	std::vector<ember::TileInfo> texture_data = ser.ReadTextureInfo(gc.cols, gc.rows);
	std::vector<ember::TileInfo> texture_data2 = ser2.ReadTextureInfo(gc.cols, gc.rows);

	gc.start_x = 0;
	gc.start_y = 0; 
	ember::Texture t("res/Dungeon_Tileset.png", &renderer);
	ember::SpriteSheet s(t, 10, 10);
	ember::TileMap map(&renderer, &input, gc);
	map.AddLayer(texture_data);
	map.AddLayer(texture_data2);

	float d = 0.0f;
	Uint32 l = 0;

	while (window.IsRunning()) {  
		d = (float)(SDL_GetTicks() - l);
		l = SDL_GetTicks();

		events.Update();
		renderer.Clear({ 67, 120, 220 });

		map.RenderTextures(s);

		renderer.Show();
	}

	ember::AssetCleanUp();

	return 0;
}