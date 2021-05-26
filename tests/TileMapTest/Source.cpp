#include "Core/Application.h"
#include "Core/Timer.h"
#include "Components/TileSystems/TileMap.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		map_serializer = new Ember::TileMapSerializer(map_info, "tiles.txt");
		map = new Ember::TileMap(renderer, events, map_info);
		map_textures = new Ember::Texture("Dungeon_Tileset.png", renderer);
		map_texture_sheet = new Ember::SpriteSheet(*map_textures, 10, 10);

		size_t top_layer = map_serializer->ReadLayeredTexture(map_info.cols, map_info.rows, map_layers);
		int layer_counter = 0;
		while (layer_counter <= top_layer) {
			map->AddLayer(map_layers[layer_counter]);
			layer_counter++;
		}
		map_control_editor = new Ember::TileMapEditor(*map);
	}

	virtual ~Sandbox() { }

	void OnUserUpdate() {
		window->Update();
		window->SetResizeable(true);

		renderer->Clear(background_color);

		map->RenderTextures(*map_texture_sheet);
		map->RenderBorder({ 100, 100, 100, 100 });

		

		renderer->Show();
	}

	bool Keyboard(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == Ember::EmberKeyCode::Escape) {
			window->Quit();
			return true;
		}
		return false;
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		dispatch.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_FUNC(Keyboard));
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };

	Ember::TileMap* map;
	Ember::GridComponents map_info;
	Ember::TileMapSerializer* map_serializer;
	std::vector<std::vector<Ember::TileInfo>> map_layers;
	Ember::Texture* map_textures;
	Ember::SpriteSheet* map_texture_sheet;
	Ember::TileMapEditor* map_control_editor;
	size_t current_map_layer;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}