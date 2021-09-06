#ifndef EMBER_H
#define EMBER_H

/*
Example code:

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
*/

#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <glm.hpp>

#define EMBER_BIND_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ember {
	SDL_DisplayMode GetDisplay();
	SDL_bool ConvertToSDLBool(bool value);
}

#endif // !EMBER_H
