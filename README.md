# Ember

Ember is an open source graphics framework for C++.

Ember holds a wide ranges of tools including event handlers, event stacks, file handlers, asset loader, 3d rendering tools and more.

Sample code:
```c++

#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "PerspectiveCameraController.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Sandbox : public Ember::Application {
public:
	void OnCreate() { 	
		Ember::RendererCommand::Init();
		Ember::Renderer::Init();
		Ember::RendererCommand::SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		camera = Ember::PerspectiveCameraController(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
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
	sandbox.Initialize("EmberApp", WINDOW_WIDTH, WINDOW_HEIGHT);

	sandbox.Run();

	return 0;
}

```

Simply call this class in `main` with it's initialize and run function and thats it! To use the API, add the desired headers like `Texture.h` and enjoy.

# Prerequisites
Having a C++ Compiler and Premake5. You can run Premake5 and build whatever build files you need. If you are building for Visual Studio 2019, there is a bat file for easy use.

# Trello
https://trello.com/b/Bnc3nLAD/ember
