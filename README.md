# Ember

Ember is an open source graphics wrapper using SDL2!

Solely created by strah19, Ember holds a wide ranges of tools including event handlers, buttons, event stacks, file handlers, asset loader, and more.

Sample code:
```c++

#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "PerspectiveCameraController.h"

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, 1280, 720);

		camera = Ember::PerspectiveCameraController(glm::vec2(1280, 720));
	}

	virtual ~Sandbox() {
		Ember::Renderer::Destroy();
	}

	void OnUserUpdate() {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.129f, 0.309f, 0.431f, 1.0f);

		cam.Update();
		Ember::Renderer::BeginScene(camera.GetCamera());

		Ember::Renderer::EndScene();

		window->Update();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		cam.OnEvent(event);
	}
private:
	Ember::PerspectiveCameraController camera;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", 1280, 720);

	sandbox.Run();

	return 0;
}

```

Simply call this class in `main` with it's initialize and run function and thats it! To use the API, add the desired headers like `Texture.h` and enjoy.

# Build
To build this project, run the `Window-Gen.bat` file in Windows (wrapper for the premake5 command) or run premake yourself if you are on a different OS.
