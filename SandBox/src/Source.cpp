#include "Core/Application.h"
#include "Core/Timer.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	}

	virtual ~Sandbox() { }

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		renderer->Rectangle(Ember::Rect({ 0, 0, 200, 200 }), { 255, 0, 0, 255 });

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
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("Ember");

	sandbox.Run();

	return 0;
}