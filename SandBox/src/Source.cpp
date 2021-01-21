#include "Core/Application.h"
#include "Assets/Texture.h"
#include "Animation/Spritesheet.h"
#include "Animation/Animation.h"
#include "TileSystems/TileMap.h"
#include "Core/OSDepStructures.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() {
		texture.Initialize("animation.png", renderer);
		spritesheet.Initialize(texture, 1, 28);
		animation_source.Initialize(spritesheet, 28);

		map_serializer = new Ember::TileMapSerializer(map_info, "tiles.txt");
		map = new Ember::TileMap(renderer, events, map_info);
		map_textures = new Ember::Texture("Dungeon_Tileset.png", renderer);
		map_texture_sheet = new Ember::SpriteSheet(*map_textures, 10, 10); 

		if (map_serializer->GetSerializer().IsFileEmpty()) {
			map_serializer->GenerateLayer(map_layers, map_info);
		}
		else {
			size_t top_layer = map_serializer->ReadLayeredTexture(map_info.cols, map_info.rows, map_layers);
			int layer_counter = 0;
			while (layer_counter <= top_layer) {
				map->AddLayer(map_layers[layer_counter]);
				layer_counter++;
			}
		}
	}
	 
	~Sandbox() {
		for (auto layer : map_layers)
			map_serializer->Save(map_info, layer);
		delete map;
		delete map_serializer;
		delete map_textures;
		delete map_texture_sheet;
	}

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		animation.Update(animation_source, 80);
		animation_source.DrawCurrentFrame(animation.CurrentFrame(), Ember::Rect({ 0, 300, 300, 300 }));

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

	Ember::Texture texture;
	Ember::SpriteSheet spritesheet;

	Ember::SpriteSheetToAnimation animation_source;
	Ember::Animation animation;

	Ember::TileMap* map;
	Ember::GridComponents map_info;
	Ember::TileMapSerializer* map_serializer;
	std::vector<std::vector<Ember::TileInfo>> map_layers;
	Ember::Texture* map_textures;
	Ember::SpriteSheet* map_texture_sheet;
	size_t current_map_layer = 0;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}