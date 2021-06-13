# Ember

Ember is an open source graphics wrapper using SDL2!

Solely created by strah19, Ember holds a wide ranges of tools including event handlers, buttons, event stacks, file handlers, asset loader, and more.

Sample code:
```c++

#include "Application.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	}

	virtual ~Sandbox() { }

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(background_color);
		
		renderer->Show();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
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

Simply call this class in `main` with it's initialize and run function and thats it! To use the API, add the desired headers like `Components/Camera.h` and enjoy.

# Build
To build this project, run the `Window-Gen.bat` file in Windows or run premake5 on your own in `premake5.lua`.
