#include "Core/Application.h"
#include "Assets/Texture.h"

class Sandbox : public Ember::Application {
public:
	~Sandbox() {
	}

	void OnCreate() { 

	}

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		renderer->Show();
	}

	bool Keyboard(Ember::KeyboardEvents& keyboard) {

		return false;
	}

	void UserDefEvent(Ember::Event& event) { 
		Ember::EventDispatcher dispatch(&event);
		dispatch.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_FUNC(Keyboard));
	}
private:
	Ember::Color background_color = { 200, 100, 50, 255 };
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}