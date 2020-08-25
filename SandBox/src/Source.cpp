#include "Ember.h"

int main(int argc, char* argv[])
{
	ember::WindowProperties properties("App", 1000, 800);
	ember::WindowsWindow window(&properties);
	ember::SDLRenderer renderer(&window);
	ember::EventHandler events(&window);

	while (window.IsRunning()) {
		events.Update();

		ember::Color background = { 67, 120, 220 };
		renderer.Clear(background);

		renderer.Show();
	}

	return 0;
}