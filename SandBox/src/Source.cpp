#include <Ember.h>
#include <chrono>
#include <ctime>    
#include <iterator>

ember::SDLProperties properties("Ember", 1700, 900, false);
ember::EmberScreen application(&properties);
ember::Font font(&application, "res/Roboto-Medium.ttf", " ", 32, { 0, 0, 0 }, 0, 0);
ember::Camera camera(&application);

struct Cell {
	ember::EmberVec2 position;
};

int cell_width = 32, cell_height = 32;
float x = 0, y = 0;
int sx, sy;
float scale_x = 1, scale_y = 1;
ember::Grid sheet(&application, 0, 0, properties.width / cell_width, properties.height / cell_height, cell_width, cell_height);
std::vector<Cell> selected_cells;

void Draw();
void Update();

int main(int argc, char* argv[])
{
	ember::CheckVersion();
	application.AddEventFlags(ember::EventFlags::kResize);
	application.AddEventFlags(ember::EventFlags::kEscape);
	application.AddEventFlags(ember::EventFlags::kJoystick);

	font.UnlockFont();

	while (application.Running) {

		application.Running = application.IterateEvents();
		application.Cursor.Update();

		ember::BeganRender(&application, 200, 200, 200);

		Update();
		Draw();

		ember::CloseRender(&application);
	}

	font.CleanUp();

	return 0;
}

void Draw() {
	sheet.RenderBorder(255, 255, 255);

	for (auto& i : selected_cells) {
		sheet.RenderRect((unsigned int)i.position.x, (unsigned int)i.position.y, 0, 100, 200);

		sheet.RenderRect((unsigned int)i.position.x - 1, (unsigned int)i.position.y, 0, 100, 200, 100);
		sheet.RenderRect((unsigned int)i.position.x + 1, (unsigned int)i.position.y, 0, 100, 200, 100);
		sheet.RenderRect((unsigned int)i.position.x, (unsigned int)i.position.y - 1, 0, 100, 200, 100);
		sheet.RenderRect((unsigned int)i.position.x, (unsigned int)i.position.y + 1, 0, 100, 200, 100);
	}

	font.ChangeFont(sheet.Hover().x, { 0, 0, 0 });
	font.Render();
}

void Update() {
	camera.WorldToScreen(x, y, sx, sy);
	sheet.ResizeGrid(sx, sy, properties.width / cell_width, properties.height / cell_height, cell_width, cell_height);

	ember::EmberVec2 click = sheet.Click(LeftMouseButton);

	if (click != ember::EmberVec2(-1.0f, -1.0f)) { 
		bool push = true;
		for (size_t i = 0; i < selected_cells.size(); i++) {
			if (click.x == selected_cells[i].position.x && click.y == selected_cells[i].position.y) {
				push = false;
				selected_cells.erase(selected_cells.begin() + i);
				break;
			}
		}
		if (push) {
			selected_cells.push_back({ click });
		}
	}

	camera.Pan(MiddleMouseButton);
}