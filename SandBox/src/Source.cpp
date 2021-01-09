#include "StartUp/Application.h"
#include "StartUp/Cursor.h"

class Sandbox : public Ember::Application {
public:
	~Sandbox() {
	}

	void OnCreate() { 

	}

	void OnUserUpdate() {
		window->Update();
		window->SetResizeable(true);

		renderer->Clear(background_color);
	

		renderer->Show();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);
	}
private:
	Ember::Color background_color = { 0, 0, 0, 255 };
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}