#include <Ember.h>

ember::SDLProperties properties("Ember", 1700, 900, false);
ember::EmberScreen application(&properties);

float f_offset_x = 0.0f;	//-application.Properties()->width / 2;
float f_offset_y = 0.0f;	//-application.Properties()->height / 2;

float f_pan_x = 0.0f;
float f_pan_y = 0.0f;
bool down_flag = true;

float f_scale_x = 1.0f;
float f_scale_y = 1.0f;

int main(int argc, char* argv[])
{
	ember::CheckVersion();
	application.AddEventFlags(ember::EventFlags::kResize);
	application.AddEventFlags(ember::EventFlags::kEscape);

	while (application.m_is_running)
	{
		application.m_is_running = application.IterateEvents();
		application.m_cursor.Update();

		float f_mouse_x = (float)application.Events()->mouse.x;
		float f_mouse_y = (float)application.Events()->mouse.y;

		if (application.Events()->mouse.down && down_flag) {
			f_pan_x = f_mouse_x;
			f_pan_y = f_mouse_y;
			down_flag = false;
		} else if(application.Events()->mouse.up) {
			down_flag = true;
		}

		if (application.Events()->mouse.down) {
			f_offset_x -= (f_mouse_x - f_pan_x) / f_scale_x;
			f_offset_y -= (f_mouse_y - f_pan_y) / f_scale_y;

			f_pan_x = f_mouse_x;
			f_pan_y = f_mouse_y;
		}

		if (application.Events()->keyBoard.name == "Q" && application.Events()->keyBoard.pressed) {
			f_scale_x *= 1.001f;
			f_scale_y *= 1.001f;
		}
		else if (application.Events()->keyBoard.name == "A" && application.Events()->keyBoard.pressed) {
			f_scale_x *= 0.999f;
			f_scale_y *= 0.999f;
		}

		ember::BeganRender(&application, 50, 50, 50);

		for (float y = 0.0f; y <= 10.0f; y++) {
			float sx = 0.0f, sy = y;
			float ex = 10.0f, ey = y;

			int p_sx, p_sy, p_ex, p_ey;
			ember::WorldToScreen(sx, sy, p_sx, p_sy, f_offset_x, f_offset_y, f_scale_x, f_scale_y);
			ember::WorldToScreen(ex, ey, p_ex, p_ey, f_offset_x, f_offset_y, f_scale_x, f_scale_y);

			ember::Line(application.Properties()->renderer, p_sx, p_sy, p_ex, p_ey, 255, 255, 255);
		}

		for (float x = 0.0f; x <= 10.0f; x++) {
			float sx = x, sy = 0.0f;
			float ex = x, ey = 10.0f;

			int p_sx, p_sy, p_ex, p_ey;
			ember::WorldToScreen(sx, sy, p_sx, p_sy, f_offset_x, f_offset_y, f_scale_x, f_scale_y);
			ember::WorldToScreen(ex, ey, p_ex, p_ey, f_offset_x, f_offset_y, f_scale_x, f_scale_y);

			ember::Line(application.Properties()->renderer, p_sx, p_sy, p_ex, p_ey, 255, 255, 255);
		}

		ember::CloseRender(&application);
	}

	return 0;
}