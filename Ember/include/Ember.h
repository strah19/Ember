#ifndef EMBER_H
#define EMBER_H

/*
Example code:

#include "Core/Application.h"

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
	void CheckVersion();
}

#endif // !EMBER_H
