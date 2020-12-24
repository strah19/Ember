#include "StartUp/Application.h"
#include "Assets/Font.h"
#include "Assets/Texture.h"

class Sandbox : public Ember::Application {
public:
	void OnClosure() {
		Ember::KeyboardTextInputEvents::StopTextInput();
		Ember::JoystickEvents::DestroyJoystick();
	}
	void OnCreate() {
		font_test.Initialize(renderer, "res/Inconsolata-Regular.ttf", "Ember!", 24, { 0, 0, 0, 255 }, 0, 0);
		texture_test.Initialize("res/texture.png", renderer);

		Ember::KeyboardTextInputEvents::StartTextInput();
		Ember::JoystickEvents::SetUpJoystick();
	}

	void OnUserUpdate() {
		window->Update();

		renderer->Clear({ 100, 170, 200, 255 });

		font_test.Render();
		texture_test.Draw(Ember::Rect({ 100, 100, 200, 200 }));

		renderer->Show();
	}

	bool KeyboardEvents(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == Ember::EmberKeyCode::Escape) {
			window->Quit();
			return true;
		}
		return false;
	}

	bool Joystick(Ember::JoystickEvents& joystick) {
		std::cout << joystick.axis << std::endl;
		return true;
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);

		dispath.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_FUNC(KeyboardEvents));
		dispath.Dispatch<Ember::JoystickEvents>(EMBER_BIND_FUNC(Joystick));
	}
private:
	Ember::Font font_test;
	Ember::Texture texture_test;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize();

	sandbox.Run();
	return 0;
}