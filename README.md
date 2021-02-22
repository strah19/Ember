# Ember

Ember is an open source graphics wrapper using SDL2!

Solely created by strah19, Ember holds a wide ranges of tools including event handlers, buttons, event stacks, file handlers, asset loader, and more.

Sample code:
```c++

#include "Core/Application.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { }

	virtual ~Sandbox() { }

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);

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
	sandbox.Initialize();

	sandbox.Run();

	return 0;
}

```

Simply call this class in main with it's initialize and run function and thats it's!
