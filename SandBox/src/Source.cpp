#include "StartUp/Application.h"
#include "Assets/Font.h"
#include "Assets/Texture.h"

class Sandbox : public Ember::Application {
public:
	void OnClosure() { }
	void OnCreate() { }

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

		renderer->Show();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);
	}
private:
	Ember::Color background_color = { 0, 0,0, 255 };
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();
	return 0;
}