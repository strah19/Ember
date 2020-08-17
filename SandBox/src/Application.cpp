#include <Ember.h>
#include <chrono>
#include <ctime>    

ember::SDLProperties properties("Ember", 1700, 900, false);
ember::EmberScreen application(&properties);

ember::TileMap map(&application, 0, 0, 20, 20, 32, 32);

ember::EmberRect rect1 = { {200, 100, 100, 100}, {0, 0} };
ember::EmberRect otherRect = { {300, 300, 100, 200}, {0, 0} };

ember::Entity entity(&application);
ember::Camera camera(&application, 0.0f, 0.0f);


ember::File file("test.txt");

void Collision() {
	ember::EmberVec2 mouse = { (float)application.Events()->mouse.x, (float)application.Events()->mouse.y };
	ember::EmberVec2 cp, cn;
	float t;
	rect1.vel = mouse - otherRect.pos.a;
	rect1.vel = rect1.vel.Normalize();
	if (ember::DynamicRectVsRect(rect1, otherRect, cp, cn, t)) {
		rect1.vel = { 0, 0 };
	}
	rect1.pos.a += rect1.vel;

	ember::RectangleFill(application.Properties()->renderer, rect1.pos.x, rect1.pos.y, rect1.pos.w, rect1.pos.h, 0, 255, 200);
	ember::RectangleFill(application.Properties()->renderer, otherRect.pos.x, otherRect.pos.y, otherRect.pos.w, otherRect.pos.h, 255, 90, 200);
	ember::FillCircle(application.Properties()->renderer, (int)cp.x, (int)cp.y, 10, 255, 0, 0);
	ember::Line(application.Properties()->renderer, cp.x, cp.y, cp.x + cn.x * 20, cp.y + cn.y * 20, 0, 0, 255);
}

SDL_Rect position = { 0, 0, 200, 200 };
ember::Button button(&application, position);
bool moving = false;
int px = 0, py = 0;

void MoveSquare()
{
	button.UpdatePosition(position.x, position.y);

	if (button.Click(LeftMouseButton)) {
		if (!moving) {
			moving = true;
			px = application.Events()->mouse.x - position.x;
			py = application.Events()->mouse.y - position.y;
		}
	} else if (moving) {
			position.x = application.Events()->mouse.x - px;
			position.y = application.Events()->mouse.y - py;
	} if (application.Events()->mouse.up) {
		moving = false;
	}

	ember::RectangleFill(application.Properties()->renderer, position.x, position.y, position.w, position.h, 0, 0, 0);
}

int main(int argc, char* argv[])
{
	ember::CheckVersion();
	application.AddEventFlags(ember::EventFlags::kResize);
	application.AddEventFlags(ember::EventFlags::kEscape);

	int x[3] = { 300, 300, 600 };
	int y[3] = { 100, 500, 500 };
	ember::Circle circles[3];
	
	for (int i = 0; i < 3; i++) {
		circles[i] = { 10, x[i], y[i] };
	}

	entity.AddComponent<ember::PositionComponent>(0.0f, 0.0f, 100.0f, 100.0f);
	entity.AddComponent<ember::CameraComponent>(&camera);
	entity.AddComponent<ember::SpriteComponent>("res/Dungeon_Tileset.png");

	while (application.m_is_running) {	

		application.m_is_running = application.IterateEvents();
		application.m_cursor.Update();

		ember::BeganRender(&application, 200, 200, 200);

		Collision();

		ember::EmberIVec2 mouse = { application.Events()->mouse.x, application.Events()->mouse.y };

		ember::Curve(application.Properties()->renderer, x, y, { 0, 0, 0, 255 });
		for (int i = 0; i < 3; i++) {
			ember::DrawCircle(application.Properties()->renderer, circles[i].x, circles[i].y, circles[i].radius, 0, 0, 0);

			if (application.Events()->mouse.down && application.Events()->mouse.id == LeftMouseButton && ember::PointToCircle(mouse, circles[i])) {
				x[i] = application.Events()->mouse.x;
				y[i] = application.Events()->mouse.y;
				circles[i].x = x[i];
				circles[i].y = y[i];
			}
		}

		map.RenderBorder(255, 255, 255);
		map.RenderRect(map.Hover().x, map.Hover().y, 255, 255, 255);

		MoveSquare();

		entity.Update();
		entity.Render();

		ember::CloseRender(&application);
	}
	 
	return 0;
}