#ifndef EMBER_H
#define EMBER_H

/*
Example code:

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

*/

#include <functional>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <sstream>

#include "Vector.h" 

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define EMBER_BIND_FUNC(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Ember {
	struct Rect {
		union {
			struct {
				int x, y, w, h;
			};
			struct {
				IVec2 pos, size;
			};
			struct {
				SDL_Rect rect;
			};
		};
	};

	struct FRect {
		union {
			struct {
				float x, y, w, h;
			};
			struct {
				Vec2 pos, size;
			};
			struct {
				SDL_FRect rect;
			};
		};
	};

	struct Color {
		union {
			struct {
				Uint8 r, g, b, a;
			};
			struct {
				SDL_Color color;
			};
		};
	};

	SDL_DisplayMode GetDisplay();
	SDL_bool ConvertToSDLBool(bool value);
	void CheckVersion();
}

#endif // !EMBER_H
