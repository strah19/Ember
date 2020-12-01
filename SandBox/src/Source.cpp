#include "Ember.h"
#include "EGui.h"

class Sandbox : public Ember::Application {
public:
	Sandbox() : Application("TileMap", false), current_map_layer(0), gui_font(renderer, "res/Roboto-Medium.ttf", " ", 14, current_text_drawing_color, 0, 0),
	 percent({ 0, 0, 0, 0 }) {
		map_serializer = new Ember::TileMapSerializer(map_info, "res/tiles.txt");
		map = new Ember::TileMap(renderer, events, map_info);
		map_textures = new Ember::Texture("res/Dungeon_Tileset.png", renderer);
		map_texture_sheet = new Ember::SpriteSheet(*map_textures, 10, 10); 

		size_t top_layer = map_serializer->ReadLayeredTexture(map_info.cols, map_info.rows, map_layers);
		int layer_counter = 0;
		while (layer_counter <= top_layer) {
			map->AddLayer(map_layers[layer_counter]);
			layer_counter++;
		}
		map_control_editor = new Ember::TileMapEditor(*map);

		camera_position.size = { window->Properties()->width, window->Properties()->height };
		user_keyboard_movement_position = { window->Properties()->width / 2, window->Properties()->height / 2 };
		clipping = { map_info.start_x, map_info.start_y, (map_info.block_width * map_info.cols) - 100, (map_info.block_height * map_info.rows) - 200 };
		percent.x = (float) map_info.start_x / window->Properties()->width * 100;
		percent.y = (float)map_info.start_y / window->Properties()->height * 100;
		percent.w = (float)((map_info.block_width * map_info.cols) - 100) / window->Properties()->width * 100;
		percent.h = (float)( (map_info.block_height * map_info.rows) - 200 ) / window->Properties()->height * 100;

		Ember::CreateGuiInterfaceInApp(g_interface, this);
		if (g_interface.CheckErrors() != Ember::InterfaceErrors::None) 
			window->Quit();

		s.Load("test.txt");
	//	s.WriteSection("newSec");
	//	s.WriteKeyValueToSection("newSec", "Key", "Value");
	}

	~Sandbox() {
		for(auto layer : map_layers)
			map_serializer->Save(map_info, layer);

		gui_font.Destroy();

		delete map;
		delete map_serializer;
		delete map_textures;
		delete map_texture_sheet;
		delete map_control_editor;
	}

	void OnUserUpdate() {
		window->Update();

		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_D])
			user_keyboard_movement_position.x += user_movement_velocity.x;
		if (state[SDL_SCANCODE_A])
			user_keyboard_movement_position.x -= user_movement_velocity.x;
		if (state[SDL_SCANCODE_W])
			user_keyboard_movement_position.y -= user_movement_velocity.y;
		if (state[SDL_SCANCODE_S])
			user_keyboard_movement_position.y += user_movement_velocity.y;

		camera_position.x = user_keyboard_movement_position.x - window->Properties()->width / 2;
		camera_position.y = user_keyboard_movement_position.y - window->Properties()->height / 2;

		map->SetCameraPosition(camera_position.pos);

		float px = (percent.x / 100) * window->Properties()->width;
		clipping.x = (int)px;

		px = (percent.y / 100) * window->Properties()->height;
		clipping.y = (int)px;

		px = (percent.w / 100) * window->Properties()->width;
		clipping.w = (int)px;

		px = (percent.h / 100) * window->Properties()->height;
		clipping.h = (int)px;

		renderer->Clear(background);

		Ember::StartFrame();

		Ember::Begin("Properties");

		Ember::End();

		Ember::Rect c = { 0, 0, 0, 0 };
		c.rect = clipping;
		renderer->Rectangle(c, { 207, 211, 212, 255 });

		SDL_RenderSetClipRect(renderer->Renderer(), &clipping);
		map->RenderTextures(*map_texture_sheet);
		map->RenderBorder({ 100, 100, 100, 100 });
		Ember::IVec2 hover = map->Hover();
		if (hover != Ember::IVec2({ -1, -1 })) {
			Ember::IVec2 pos = map->ConvertPoints(hover.x, hover.y);
			renderer->Border({ pos.x, pos.y, map_info.block_width, map_info.block_height }, { 255, 0, 0, 100 });
		}
		SDL_RenderSetClipRect(renderer->Renderer(), NULL);

		gui_font.SetPosition(current_text_drawing_position.x, current_text_drawing_position.y);
		std::stringstream moving_position_list;
		moving_position_list << "Tile Positon: " << map_info.start_x - camera_position.x << "   " << map_info.start_y - camera_position.y;
		DrawText(moving_position_list.str());
		
		moving_position_list.str(std::string());

		moving_position_list << "Camera Position: " << camera_position.x << "    " << camera_position.y;
		gui_font.SetPosition(current_text_drawing_position.x, current_text_drawing_position.y + y_offset_gui_list_value);
		DrawText(moving_position_list.str());

		moving_position_list.str(std::string());

		moving_position_list << "Tile Size: " << map_info.block_width << "    " << map_info.block_height;
		gui_font.SetPosition(current_text_drawing_position.x, current_text_drawing_position.y + y_offset_gui_list_value * 2);
		DrawText(moving_position_list.str());

		moving_position_list.str(std::string());

		moving_position_list << "Current Map Layer: " << current_map_layer;
		gui_font.SetPosition(current_text_drawing_position.x, current_text_drawing_position.y + y_offset_gui_list_value * 3);
		DrawText(moving_position_list.str());

		renderer->Rectangle({ window->Properties()->width - (editor_tile_size.x * map_texture_sheet->Size().x), 0, editor_tile_size.x * map_texture_sheet->Size().x , editor_tile_size.y * map_texture_sheet->Size().y }, { 40, 40, 40, 255 });
		map_control_editor->RenderSpriteSheet(*map_texture_sheet, {window->Properties()->width - (editor_tile_size.x * map_texture_sheet->Size().x), 0 }, Ember::ButtonIds::RightMouseButton, editor_tile_size);
		map_control_editor->UpdateMap(map_layers[current_map_layer], Ember::ButtonIds::LeftMouseButton, camera_position.pos);
		map->UploadTextureVector(map_layers[current_map_layer], current_map_layer); 

		map_control_editor->DeleteTile(map_layers[current_map_layer], SDL_SCANCODE_P);


		renderer->Show();
	}

	template<typename T>
	void DrawText(const T& value) {
		gui_font.UnlockFont();
		gui_font.ChangeFont(value, current_text_drawing_color);
		gui_font.Render();
	}

	bool KeyboardEvents(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == SDL_SCANCODE_ESCAPE) {
			window->Quit();
			return true;
		}
		else if (keyboard.scancode == SDL_SCANCODE_N) {
			map_layers.push_back(std::vector<Ember::TileInfo>());
			current_map_layer++;
			map_layers[current_map_layer] = map_serializer->AddNewLayer(map_info.cols, map_info.rows);
			map->AddLayer(map_layers[current_map_layer]);
		}
		else if (keyboard.scancode == SDL_SCANCODE_Q) {
			if (current_map_layer < map_layers.size() - 1)
				current_map_layer++;
		}
		else if (keyboard.scancode == SDL_SCANCODE_Z) {
			if (current_map_layer > 0)
				current_map_layer--;
		}
		
		return false;
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);

		dispath.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_EVENT(KeyboardEvents));
	}
private: 
	Ember::Rect camera_position = { 0, 0, 0, 0 };
	SDL_Rect clipping;

	Ember::IVec2 user_movement_velocity = { 2, 2 };
	Ember::IVec2 user_keyboard_movement_position = { 0, 0 };

	Ember::TileMap* map;
	Ember::GridComponents map_info;
	Ember::TileMapSerializer* map_serializer;
	std::vector<std::vector<Ember::TileInfo>> map_layers;
	Ember::Texture* map_textures;
	Ember::SpriteSheet* map_texture_sheet;
	Ember::TileMapEditor* map_control_editor;
	size_t current_map_layer;

	Ember::Font gui_font;
	Ember::IVec2 current_text_drawing_position = { 0, 0 };

	Ember::Color background = { 100, 120, 190, 255 };
	int y_offset_gui_list_value = 20;
	Ember::IVec2 user_size{ 5, 5 };
	Ember::Color current_text_drawing_color = { 255, 255, 255, 255 };

	Ember::IVec2 editor_tile_size = { 32, 32 };

	Ember::FRect percent;

	Ember::EGuiInterface g_interface;
	Ember::CinderStructure s;

	bool in_editor_mode = false;
};

int main(int argc, char** argv) 
{
	Sandbox sandbox;

	sandbox.Run();

	return 0;  
}