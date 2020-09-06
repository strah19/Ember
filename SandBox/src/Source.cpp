#include "Core/Ember.h"

int main(int argc, char* argv[])
{
	ember::WindowProperties properties("App", 960, 800);
	ember::WindowsWindow window(&properties);
	ember::Renderer2D renderer(&window);
	ember::EventHandler events(&window);
	ember::Input input(&events);

	ember::InitializeAssets();

	ember::GridComponents gc;
	ember::TileMapSerializer ser(gc, "tiles.txt");

	std::vector<ember::TileInfo> texture_data = ser.ReadTextureInfo(gc.cols, gc.rows);

	gc.start_x = 0;
	gc.start_y = 0; 
	ember::Texture t("res/Dungeon_Tileset.png", &renderer);
	ember::SpriteSheet s(t, 10, 10); 
	ember::TileMap map(&renderer, &input, gc);
	map.AddLayer(texture_data);

	ember::TileMapEditor editor(map);

	float d = 0.0f;
	Uint32 l = 0;

	ember::Camera game_camera(0.0f, 0.0f);
	ember::Camera editor_camera(0.0f, 0.0f);

	std::vector<ember::IVec2> collisions;

	ember::Entity player;
	player.AddComponent<ember::PositionComponent>(properties.width / 2, properties.height / 2, 48, 48);
	player.AddComponent<ember::SpriteComponent>("res/player.png", &renderer);
	player.AddComponent<ember::DebugComponent>(&input);

	ember::IVec2 transform = { 0, 0 };
	int lx = 0, ly = 0;

	ember::File serializer("tilecoll.txt");
	std::vector<bool> texture_ids;
	unsigned int texture_id_num = 30 * 25;
	serializer.DoEachWord([&](std::string& word, unsigned int counter) {
		if (counter < texture_id_num)
			texture_ids.push_back({ (bool) std::stoi(word) });
	});

	map.Personalize([&](int x, int y, int bw, int bh) {
		if (texture_ids[30 * (y / 96) + (x / 96)]) {
			collisions.push_back({ x, y });
		}
	});

	while (window.IsRunning()) {  
		d = (float)(SDL_GetTicks() - l);
		l = SDL_GetTicks();

		events.Update();
		renderer.Clear({ 37, 19, 26 });

		map.ResizeGrid(gc);
		map.RenderTextures(s);

#ifdef  EMBER_DEBUG_MODE
		/*
		int ex, ey;
		editor_camera.WorldToScreen(600.0f, 0.0f, ex, ey);
		if (input.PressedKey() != "P") {
			editor_camera.Pan(input, 2);
			renderer.Rectangle({ ex - 2, ey - 2, s.Size().x * gc.block_width + 4, s.Size().y * gc.block_height + 4 }, { 200, 200,200, 255 });
			editor.RenderSpriteSheet(s, { ex, ey }, 3);
			editor.UpdateMap(texture_data);
			map.UploadTextureVector(texture_data, 0);
		}
		*/

		ember::IVec2 c = map.Click(1);

		if (c != ember::IVec2(-1, -1)) {
			collisions.push_back(map.ConvertPoints(c.x, c.y));
			texture_ids[30 * c.y + c.x] = true;
		}		
#endif 
#ifndef EMBER_DEBUG_MODE
		lx = transform.x;
		ly = transform.y;

		if (input.KeyboardState(SDL_SCANCODE_D)) {
			transform += {2, 0};
			player.GetComponent<ember::SpriteComponent>().SetFlipMode(SDL_FLIP_NONE);
		}
		if (input.KeyboardState(SDL_SCANCODE_S)) {
			transform += {0, 2};
		}
		if (input.KeyboardState(SDL_SCANCODE_W)) {
			transform += {0, -2};
		}
		if (input.KeyboardState(SDL_SCANCODE_A)) { 
			transform += {-2, 0};
			player.GetComponent<ember::SpriteComponent>().SetFlipMode(SDL_FLIP_HORIZONTAL);
		}

		for (size_t i = 0; i < collisions.size(); i++) {
			if (ember::RectVsRect(ember::FRect({ (float)(transform.x + player.GetComponent<ember::PositionComponent>().Position().x), 
												 (float)(transform.y + player.GetComponent<ember::PositionComponent>().Position().y),
				(float) player.GetComponent<ember::PositionComponent>().Position().w, (float) player.GetComponent<ember::PositionComponent>().Position().h }),
				{ (float)collisions[i].x, (float)collisions[i].y, 96.0f, 96.0f })) {
				transform.x = lx;
				transform.y = ly;
			}
		}

		int tx, ty;
		game_camera.WorldToScreen(-transform.x, -transform.y, tx, ty);
		gc.start_x = tx;
		gc.start_y = ty;
		map.ResizeGrid(gc);
#endif
		player.Render();
		player.Update();

		renderer.Show();
	}

	ser.Save(gc, texture_data);

	serializer.EmptyFile();
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 30; j++) {
			serializer.Write(texture_ids[30 * i + j]);
			serializer.Write(" ");
		}
		serializer.Write("\n");
	}

	ember::AssetCleanUp();

	return 0;
}