# Ember

Ember is an open source graphics framework for C++.

Ember holds a wide ranges of tools including event handlers, event stacks, file handlers, asset loader, 3d rendering tools and more.

Sample code:
```c++
#include "Application.h"
#include "Renderer.h"
#include "RendererCommands.h"
#include "OrthoCameraController.h"
#include "Geometry.h"
#include "FrameBuffer.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

class Sandbox : public Ember::Application {
public:
	void OnCreate() {
		Ember::RendererCommand::Init();
		Ember::RendererCommand::SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

		camera = Ember::OrthoCameraController(glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));

		imgui = new Ember::ImGuiLayer(window, event_handler);
		PushLayer(imgui);
	}

	virtual ~Sandbox() { }

	void OnUserUpdate(float delta) {
		Ember::RendererCommand::Clear();
		Ember::RendererCommand::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		camera.Update();
		renderer->BeginScene(&camera.GetCamera());

		Ember::Quad::Renderer(renderer);
		Ember::Quad::DrawQuad({ 0, 0, 0 }, { 1, 1 }, { 1, 0, 0, 1 });
		Ember::Quad::DrawQuad({ 1, 0, 0 }, { 1, 1 }, { 1, 1, 0, 1 });

		renderer->EndScene();
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispatch(&event);
		camera.OnEvent(event);
	}

	void OnGuiUpdate() { }
private:
	Ember::OrthoCameraController camera;
};

int main(int argc, char** argv) {
	Sandbox sandbox;
	sandbox.Initialize("EmberApp", WINDOW_WIDTH, WINDOW_HEIGHT);

	sandbox.Run();

	return 0;
}

```

Simply call this class in `main` with it's initialize and run function and thats it! To use the API, add the desired headers like `Texture.h` and enjoy.

![image not found](https://github.com/strah19/Ember/blob/master/docs/example.png?raw=true)


# Dependencies 
All these can actually be found in libs folder (besides the compiler) but here they are anyways:
<ul>
	<li>A compiler that supports C++17</li>
	<li><a href = "https://premake.github.io">Premake5</a> for building the project</li>
	<li><a href = "https://www.libsdl.org/download-2.0.php">SDL2</a></li>
	<li><a href = "https://www.libsdl.org/projects/SDL_mixer">SDL Mixer</a></li>
	<li><a href = "https://www.libsdl.org/projects/SDL_image">SDL Image</a></li>
	<li><a href = "https://glad.dav1d.de">GLAD</a></li>
	<li><a href = "https://github.com/g-truc/glm">GLM</a></li>
	<li><a href = "https://www.freetype.org">Freetype</a></li>
</ul>
